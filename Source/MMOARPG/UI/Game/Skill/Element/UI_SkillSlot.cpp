#include "UI_SkillSlot.h"
#include "Components/Button.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "DragDrop/UI_ICODragDrog.h"
#include "Styling/SlateBrush.h"
#include "../../../../Core/Game/Character/MMOARPGCharacter.h"
#include "../UI_UnderSkillGroup.h"
#include "MMOARPG/Core/Game/Abilities/MMOARPGAttributeSet.h"
#include "Tip/UI_Tip.h"
#include "../../../../Core/Game/MMOARPGHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ThreadManage.h"

int32 UUI_SkillSlot::PlayerSkillNumber = 0;

#define LOCTEXT_NAMESPACE "UUI_SkillSlot"

UUI_SkillSlot::UUI_SkillSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bMappingKey(false)
	, KeyNumber(INDEX_NONE)
	, bWaitUntil(true)
{
	SkillType = EMMOARPGSkillType::NONE_SKILLS;
}

void UUI_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibilityBorderHeight(false);

	if (bMappingKey) {
		//映射键位
		KeyNumber = ++PlayerSkillNumber;

		// 绑定键位lambda
		auto BindInput = [&](const FString& InPlayerSkillName/*这个按键行为的名字,例如ShotsFire开枪*/) {
			GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(*InPlayerSkillName, IE_Pressed, this, &UUI_Slot::OnClickedWidget);// 键鼠输入绑定回调
			GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(*InPlayerSkillName, IE_Released, this, &UUI_Slot::OnReleasedClickedWidget);// 键鼠输入绑定回调
		};

		switch (PlayerSkillNumber) {
			/* 键位1~5均执行一段 通用技能逻辑*/
			case 1://GENERAL_SKILLS
			case 2://GENERAL_SKILLS
			case 3://GENERAL_SKILLS
			case 4://GENERAL_SKILLS
			case 5://GENERAL_SKILLS
			{
				// 设定技能分型
				SkillType = EMMOARPGSkillType::GENERAL_SKILLS;
				// 保存键位名字,从整形数字转化为字符
				KeyString = FString::FromInt(KeyNumber);
				// 绑定键位Lambda
				BindInput(FString::Printf(TEXT("PlayerSkill_%i"), PlayerSkillNumber));
				break;
			}
			// 键位6,此键位只能存储 从天而降类型技能
			case (int32)EMMOARPGSkillType::DROP_FROM_THE_CLOUDS_SKILL:
			{
				SkillType = EMMOARPGSkillType::DROP_FROM_THE_CLOUDS_SKILL;
				KeyString = TEXT("R");
				BindInput(TEXT("DropFromTheClouds"));
				UUI_SlotElement::SlotIcon->SetIsEnabled(false);
				bWaitUntil = false;
				break;
			}
			// 键位7,此键位只能存储 闪避类型技能
			case (int32)EMMOARPGSkillType::DODGE_SKILL:
			{
				SkillType = EMMOARPGSkillType::DODGE_SKILL;
				KeyString = TEXT("MR");
				BindInput(TEXT("MouseRightClick"));
				break;
			}
			// 键位8,此键位只能存储 地面连击类型技能
			case (int32)EMMOARPGSkillType::COMBO_GROUND_SKILL:
			{
				SkillType = EMMOARPGSkillType::COMBO_GROUND_SKILL;
				KeyString = TEXT("ML");
				BindInput(TEXT("MouseClick"));
				break;
			}
			// 键位9,此键位只能存储 滞空连击技能
			case (int32)EMMOARPGSkillType::COMBO_AIR_SKILL:
			{
				SkillType = EMMOARPGSkillType::COMBO_AIR_SKILL;
				KeyString = TEXT("ML");
				BindInput(TEXT("MouseClick"));
				break;
			}
			// 键位10,此键位只能存储 条件类型技能
			case (int32)EMMOARPGSkillType::CONDITIONAL_SKILLS:
			{
				SkillType = EMMOARPGSkillType::CONDITIONAL_SKILLS;
				KeyString = TEXT("F");
				BindInput(TEXT("ConditionalSkill"));
				UUI_SlotElement::SlotIcon->SetIsEnabled(false);
				bWaitUntil = false;
				break;
			}
			// 键位11,此键位只能存储 冲刺型技能
			case (int32)EMMOARPGSkillType::SPRINT_SKILLS:
			{
				SkillType = EMMOARPGSkillType::SPRINT_SKILLS;
				KeyString = TEXT("E");
				BindInput(TEXT("Sprint"));

				// 重置映射键位
				PlayerSkillNumber = 0;
				break;
			}
		}

		// 设置Key名称
		UUI_Slot::SetKeyName(KeyString);
	}

	//创建提示
	CreateTip();
}

void UUI_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	/** 条件技能分型射线判断. */
	if (SkillType == EMMOARPGSkillType::DROP_FROM_THE_CLOUDS_SKILL) {// 条件技能-从天而降
		if (this->SlotInfo.IsVaild()) {// 技能槽内有技能
			/* 打射线操作.*/
			if (AMMOARPGCharacterBase* InCharacter = GetPawn<AMMOARPGCharacterBase>()) {
				if (IsCooldown()) {// 是否在CD中
					if (IsCost()) {// 是否负担得起发动技能的消耗(蓝是否足够)
						
						// 准备开始和结束的位置
						FVector StartPos = InCharacter->GetActorLocation();
						float HalfHeight = InCharacter->GetCapsuleHalfHeight();
						StartPos.Z -= HalfHeight;
						FVector Dir = -InCharacter->GetActorUpVector();
						FVector EndPos = StartPos + (Dir * 4000.f);

						// 准备好射线的内容
						FHitResult HitResult;
						TArray<AActor*> ActorsToIgnore = { InCharacter };

						// 打射线
						UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartPos, EndPos,
							ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore,
							EDrawDebugTrace::Type::None,
							HitResult, true);

						if (HitResult.bBlockingHit) {
							float ChestDistance = FVector::Distance(StartPos, HitResult.Location);
							if (ChestDistance > 60.f) {
								UUI_SlotElement::SlotIcon->SetIsEnabled(true);
								bWaitUntil = true;
							}
							else {
								UUI_SlotElement::SlotIcon->SetIsEnabled(false);
								bWaitUntil = false;
							}
							//打印
							//	Print(FString::SanitizeFloat(ChestDistance));
						}
						else {
							UUI_SlotElement::SlotIcon->SetIsEnabled(false);
							bWaitUntil = false;
						}
					}
				}
			}
		}
	}
}

void UUI_SkillSlot::OnClickedWidget()
{
	UUI_Base::Print(FString::FromInt(KeyNumber));

	if (UWidget::bIsEnabled == true && bWaitUntil == true) {// 控件可以接触可以交互
		if (KeyNumber > 0) {
			if (UUI_Slot::IsCooldown()) {// 是否 在处在CD中
				if (!IsShieldSkill()) {// 是否 未屏蔽技能输入
					if (IsCost()) {// 是否 足以消耗
						if (AMMOARPGCharacter* InCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacter>()) {
							/* 判断左键按下后的Combo技能是否执行生效.*/
							if (KeyString == TEXT("ML")) {
								if (SkillType == EMMOARPGSkillType::COMBO_AIR_SKILL) {
									if (!InCharacter->IsAir()) {
										return;
									}
								}
								else if (SkillType == EMMOARPGSkillType::COMBO_GROUND_SKILL) {
									if (InCharacter->IsAir()) {
										return;
									}
								}
							}
							//
							if (InCharacter->GetActionState() == ECharacterActionState::FIGHT_STATE) {// 仅当进入战斗行为状态
								// 服务端执行技能形式的技能攻击(需指定一个槽号)
								InCharacter->SKillAttackOnServer(KeyNumber);
							}
							else {
								// 警示 未进入战斗姿态
								UUI_Base::WarningPrint(LOCTEXT("ActionState_Key", "Must be in fight state."));
							}
						}
					}
					else {
						// 警示 技能不满足足以消耗的条件
						UUI_Base::WarningPrint(LOCTEXT("Cost_Key", "Mana deficiency."));
					}
				}
			}
			else {
				// 警示 本技能尚且处于冷却中
				UUI_Base::WarningPrint(LOCTEXT("CD_Key", "This skill is not ready yet."));
			}
		}
	}
}

void UUI_SkillSlot::OnReleasedClickedWidget()
{
	if (KeyNumber > 0) {
		if (AMMOARPGCharacter* InCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacter>()) {
			// 是否处于战斗状态下
			if (InCharacter->GetActionState() == ECharacterActionState::FIGHT_STATE) {
				// 在服务端中止已激发的技能
				InCharacter->ReleaseSKillAttackOnServer(KeyNumber);
			}
			else {
				WarningPrint(LOCTEXT("ActionState_Key", "Must be in fight state."));
			}
		}
	}
}

#pragma region 对外接口
void UUI_SkillSlot::Update(const FName& InTagName, UTexture2D* InTexture, float InCost /*= 0.f*/, EMMOARPGSkillType InSkillType /*= EMMOARPGSkillType::GENERAL_SKILLS*/)
{
	this->SetIcon(InTexture);
	this->SlotInfo.Tags = InTagName;// 注册技能名字为入参.
	this->SlotInfo.Cost = InCost;// 指派为特定的消耗值
	this->SlotInfo.SkillType = InSkillType;// 注册为指定的技能分类类型
}

UTexture2D* UUI_SkillSlot::GetIcon()
{
	return Cast<UTexture2D>(SlotIcon->Brush.GetResourceObject());
}

void UUI_SkillSlot::SetIcon(UTexture2D* InNewTexture)
{
	SlotIcon->SetBrushFromTexture(InNewTexture);// 设置纹理
	SlotIcon->SetVisibility(ESlateVisibility::HitTestInvisible);// 蓝图内默认隐藏,这里需要手动显示
}

void UUI_SkillSlot::ResetIcon()
{
	SlotIcon->SetBrushFromTexture(nullptr);
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_SkillSlot::HiddenIcon()
{
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_SkillSlot::VisibleIcon()
{
	SlotIcon->SetVisibility(ESlateVisibility::Visible);
}
#pragma endregion 对外接口


#pragma region 覆写侦测拖拽手势系统函数
/** 侦测鼠标键按下 */
FReply UUI_SkillSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent()) {// 判断是鼠标右键执行了触摸
		FReply Reply = FReply::Handled();

		TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
		if (SlateWidgetDrag.IsValid()) {// 检查触碰的UMG是有意义的
			// 此刻标记并侦测为拖拽行为
			Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
			return Reply;
		}
	}

	return FReply::Handled();
}

/** 识别到是拖拽行为 */
void UUI_SkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (this->ICODragDrogClass != nullptr && this->GetSlotInfo().IsVaild()) {// 允许产生拖拽行为的前提: 拖拽ICON显示元素类不为空; 且名称必须有意义
		if (UUI_ICODragDrog* ICODragDrogUMGIns = CreateWidget<UUI_ICODragDrog>(GetWorld(), ICODragDrogClass)) {// 构建被鼠标右键拖拽出来的技能Logo
			/* New一个拖拽操作行为*/
			if (UDragDropOperation* InDropOperation = NewObject<UDragDropOperation>(GetTransientPackage(), UDragDropOperation::StaticClass())) {
				InDropOperation->SetFlags(RF_StrongRefOnFrame);// 防止GC
				ICODragDrogUMGIns->DrawICON(Cast<UTexture2D>(SlotIcon->Brush.GetResourceObject()));// 设置图图片

				InDropOperation->DefaultDragVisual = ICODragDrogUMGIns;// 填充拖拽行为
				InDropOperation->Payload = this;// 填充拖拽行为
				OutOperation = InDropOperation;// 注册出参

				// 拖拽的时候 默认隐藏技能Logo
				HiddenIcon();

				/* 当识别到是拖拽行为的时候, 就可以产生技能槽的高亮了,而且必须强制匹配技能分型.*/
				if (AMMOARPGHUD* InMMOARPGHUD = UUI_Base::GetHUD<AMMOARPGHUD>()) {
					InMMOARPGHUD->BorderHeightDisplayDelegate.Broadcast(SlotInfo.SkillType);
				}
			}
		}
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

/** 拖拽行为松开 */
bool UUI_SkillSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	// 拖拽结果
	bool bDrop = false;

	if (InOperation) {// 接收到有入参拖拽操作
		if (AMMOARPGCharacter* InCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacter>()) {
			/* 接收到上一步拖拽行为传入的Operation, 拿到入参拖拽的UMG控件*/
			if (UUI_SkillSlot* MyInventorySlot = Cast<UUI_SkillSlot>(InOperation->Payload)) {// 拖拽出来的技能插槽实例

				/// /** I. 技能插槽交换行为 */
				if (MyInventorySlot->GetSlotInfo().IsVaild() && this->GetSlotInfo().IsVaild()) {// 拖拽出来的插槽技能信息和 自己本身的插槽技能信息 名字都有意义

					if (MyInventorySlot->IsSkillTableSlot() && this->IsSkillTableSlot()) {// Page->Page
						// 在客户端方面的"交换行为"-拖拽操作实质逻辑
						UpdateSwap(MyInventorySlot);
					}
					/* 仅允许同类型的通用技能互换*/
					else if ((MyInventorySlot->GetSlotInfo().SkillType == EMMOARPGSkillType::GENERAL_SKILLS) && (MyInventorySlot->GetSlotInfo().SkillType == GetSlotInfo().SkillType)) {

						// 在服务端方面的"交换行为"-拖拽操作实质逻辑
						UpdateSwapByServer(MyInventorySlot, InCharacter);
						// 在客户端方面的"交换行为"-拖拽操作实质逻辑
						UpdateSwap(MyInventorySlot);
					}
					else if (MyInventorySlot->GetSlotInfo().SkillType != EMMOARPGSkillType::NONE_SKILLS) {
						if ((MyInventorySlot->GetSlotInfo().SkillType == this->SkillType && MyInventorySlot->IsSkillTableSlot()) || // Page->Slot
							(!MyInventorySlot->IsSkillTableSlot() && MyInventorySlot->GetSlotInfo().SkillType == this->GetSlotInfo().SkillType)) {//Slot->Page 
							// 在服务端方面的"交换行为"-拖拽操作实质逻辑
							UpdateSwapByServer(MyInventorySlot, InCharacter);
							// 在客户端方面的"交换行为"-拖拽操作实质逻辑
							UpdateSwap(MyInventorySlot);
						}
						else {
							WarningPrint(LOCTEXT("Icon_Drag", "Skills and slot types are different."));
							MyInventorySlot->VisibleIcon();
						}
					}
					else {
						WarningPrint(LOCTEXT("Icon_Drag", "Skills and slot types are different."));
						MyInventorySlot->VisibleIcon();
					}
				}
				/// /** II. 技能插槽移动; // 拖拽出来的有技能, 自己作为接收方没有技能,这种行为称之为技能移动 */
				else if (MyInventorySlot->GetSlotInfo().IsVaild() && !this->GetSlotInfo().IsVaild()) {

					/** Page->Page */
					if (MyInventorySlot->IsSkillTableSlot() && IsSkillTableSlot()) {// Page->Page
						// 在客户端方面的"移动行为"-拖拽操作实质逻辑
						UpdateMoveTo(MyInventorySlot);
					}
					/** 对方是通用类型技能, 而自己必须是无类型技能的键位 */
					else if (MyInventorySlot->GetSlotInfo().SkillType == EMMOARPGSkillType::GENERAL_SKILLS && SkillType == EMMOARPGSkillType::NONE_SKILLS) {

						// 在服务端方面的"移动行为"-拖拽操作实质逻辑
						UpdateMoveToByServer(MyInventorySlot, InCharacter);
						// 在客户端方面的"移动行为"-拖拽操作实质逻辑
						UpdateMoveTo(MyInventorySlot);
					}
					/** 对方的类型不可以为无类型 */
					else if (MyInventorySlot->GetSlotInfo().SkillType != EMMOARPGSkillType::NONE_SKILLS) {

						if ((MyInventorySlot->GetSlotInfo().SkillType == this->SkillType && MyInventorySlot->IsSkillTableSlot()) ||// 从Page->Slot;
							(this->IsSkillTableSlot() && this->SkillType == EMMOARPGSkillType::NONE_SKILLS)) {// 从Slot->Page
							// 在服务端方面的"移动行为"-拖拽操作实质逻辑
							UpdateMoveToByServer(MyInventorySlot, InCharacter);
							// 在客户端方面的"移动行为"-拖拽操作实质逻辑
							UpdateMoveTo(MyInventorySlot);
						}
						else {// 操作失败警告打印
							WarningPrint(LOCTEXT("Icon_Drag", "Skills and slot types are different."));
							MyInventorySlot->VisibleIcon();
						}
					}
					/**  */
					else {
						WarningPrint(LOCTEXT("Icon_Drag", "Skills and slot types are different."));
						MyInventorySlot->VisibleIcon();
					}
				}
				/// /** III. 其他行为(比如拖拽失败了) */
				else {
					// 让拖拽Logo在过程中显示出来
					MyInventorySlot->VisibleIcon();
				}

				bDrop = true;
			}
		}
	}
	
	// 松开的时候会 取消技能槽的高亮
	if (AMMOARPGHUD* InMMOARPGHUD = GetHUD<AMMOARPGHUD>()) {
		InMMOARPGHUD->ResetHeightDisplayDelegate.Broadcast();
	}

	return bDrop;
}

/** 鼠标刚接触本UI */
void UUI_SkillSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	// 重置Tip显示
	ResetTipText();

	// 有文本就显示tip没文本就关闭tip
	if (IsText()) {
		SetVisibilityTipWidget(true);
	}
	else {
		SetVisibilityTipWidget(false);
	}
}

/** 鼠标刚离开本UI */
void UUI_SkillSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

/// 在服务端方面的"交换行为"-拖拽操作实质逻辑
void UUI_SkillSlot::UpdateSwapByServer(UUI_SkillSlot* MyInventorySlot, AMMOARPGCharacter* InCharacter)
{
	/* 1.1 服务器表现 */
	if (InCharacter && MyInventorySlot) {
		if (!MyInventorySlot->IsSkillTableSlot() && !this->IsSkillTableSlot()) {// 在横框里, 2个互换
			// 通知服务端 这2个槽号的槽Swap
			InCharacter->SillSlotSwap(this->KeyNumber, MyInventorySlot->KeyNumber);
		}
		else if (MyInventorySlot->IsSkillTableSlot() && !IsSkillTableSlot()) {// 对方是从技能页拖出来的,而自己是在技能框里的
			// 从技能页拖拽一个移动到技能框里
			InCharacter->SKillTableSlotSwapSkillSlot(KeyNumber, MyInventorySlot->GetSlotInfo().Tags);
		}
		else if (!MyInventorySlot->IsSkillTableSlot() && IsSkillTableSlot()) {// 对方拖过来的来自技能框, 自己作为接收方是技能页
			// 从技能框交换到技能页里
			InCharacter->SKillSlotSwapSkillTable(MyInventorySlot->KeyNumber, GetSlotInfo().Tags);
		}
	}
}

///  在客户端方面的"交换行为"-拖拽操作实质逻辑
void UUI_SkillSlot::UpdateSwap(UUI_SkillSlot* MyInventorySlot)
{
	/* 1.2 客户端表现的效果.*/
	if (MyInventorySlot) {
		// 技能效果
		{
			// 先缓存 "右键拖拽Logo"的纹理和名字
			UTexture2D* TmpTexture = MyInventorySlot->GetIcon();
			//FName TmpTags = MyInventorySlot->GetSlotInfo().Tags;
			FWidgetSlotInfo DraggedWidgetInfo = MyInventorySlot->GetSlotInfo();

			// 给 "右键拖拽Logo" 重新写入
			MyInventorySlot->SetIcon(GetIcon());
			MyInventorySlot->GetSlotInfo() = DraggedWidgetInfo;

			// 将自己的技能插槽信息更换为之前缓存的那一份
			GetSlotInfo() = DraggedWidgetInfo;
			SetIcon(TmpTexture);
		}

		// CD效果
		{
			FSlotBuild SlotBuildTmp = MyInventorySlot->SlotBuild;
			MyInventorySlot->SlotBuild = this->SlotBuild;
			this->SlotBuild = SlotBuildTmp;
			this->StartUpdateCD();
			MyInventorySlot->StartUpdateCD();
			MyInventorySlot->ClearCD();
			this->ClearCD();
		}
	}
}

/// 在服务端方面的"移动行为"-拖拽操作实质逻辑
void UUI_SkillSlot::UpdateMoveToByServer(UUI_SkillSlot* MyInventorySlot, AMMOARPGCharacter* InCharacter)
{
	/* 2.1 服务器表现 */
	if (InCharacter && MyInventorySlot) {
		if (!MyInventorySlot->IsSkillTableSlot() && !IsSkillTableSlot()) {// 双方都在技能框内
			// 通知服务端 这2个槽号的槽移动
			InCharacter->SKillSlotMoveToNewSlot(MyInventorySlot->KeyNumber, KeyNumber);
		}
		else if (MyInventorySlot->IsSkillTableSlot() && !IsSkillTableSlot()) {// 对方是从技能页拖出来的,而自己是在技能框里的
			// 从技能页里面移动过来一个技能到空的技能表里面
			InCharacter->SKillTableSlotMoveToSkillSlot(MyInventorySlot->GetSlotInfo().Tags, KeyNumber);
		}
		else if (!MyInventorySlot->IsSkillTableSlot() && IsSkillTableSlot()) {// 对方是技能框,自己是技能页
			InCharacter->SKillSlotMoveToSkillTable(MyInventorySlot->KeyNumber);/** 从横框到技能页: 移动 */
		}
	}
}

/// 在客户端方面的"移动行为"-拖拽操作实质逻辑
void UUI_SkillSlot::UpdateMoveTo(UUI_SkillSlot* MyInventorySlot)
{
	/* 2.2 客户端表现 */
	if (MyInventorySlot) {
		// 技能效果
		{
			// 设置一下自身
			this->GetSlotInfo() = MyInventorySlot->GetSlotInfo();
			this->SetIcon(MyInventorySlot->GetIcon());
			// 把拖拽的实例复位掉
			MyInventorySlot->ResetIcon();
			MyInventorySlot->GetSlotInfo().Reset();
		}

		// CD效果
		{
			StartUpdateCD(MyInventorySlot->SlotBuild.Cooldown);// 标记CD UI开始
			SetMaxCD(MyInventorySlot->SlotBuild.MaxCooldown);// 设置最大CD
			MyInventorySlot->FoceClearCD();
		}
	}
}

#pragma endregion 覆写侦测拖拽手势系统函数

// 是否屏蔽了技能输入
bool UUI_SkillSlot::IsShieldSkill()
{
	if (UUI_UnderSkillGroup* InUnderSkillGroup = GetParents<UUI_UnderSkillGroup>()) {
		return InUnderSkillGroup->IsShieldSkill();
	}

	return false;
}

// 技能是否满足 足以消耗
bool UUI_SkillSlot::IsCost()
{
	if (GetWorld() && GetWorld()->GetFirstPlayerController()) {
		if (AMMOARPGCharacterBase* InPawn = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacterBase>()) {
			if (UMMOARPGAttributeSet* InAttribute = InPawn->GetAttribute()) {
				return InAttribute->GetMana() >= SlotInfo.Cost;
			}
		}
	}

	return false;
}

#pragma region 关于Tip悬浮框的一些接口
void UUI_SkillSlot::CreateTip()
{
	if (TipClass) {
		if (UUI_Tip* InTip = CreateWidget<UUI_Tip>(GetWorld(), TipClass)) {
			SetToolTip(InTip);
		}
	}
}

void UUI_SkillSlot::SetVisibilityTipWidget(bool bShow)
{
	if (UUI_Tip* InTip = Cast<UUI_Tip>(ToolTipWidget.Get())) {
		if (bShow) {
			InTip->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			InTip->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUI_SkillSlot::SetVisibilityBorderHeight(bool bShow)
{
	if (bShow) {
		BorderHeight->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		BorderHeight->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_SkillSlot::SetTipTextContent(const FText& InText)
{
	SlotInfo.Tip = InText;
}

void UUI_SkillSlot::SetTipText(const FText& InText)
{
	if (UUI_Tip* InTip = Cast<UUI_Tip>(ToolTipWidget.Get())) {
		InTip->SetText(InText);
	}
}

void UUI_SkillSlot::ResetTipText()
{
	SetTipText(SlotInfo.Tip);
}

void UUI_SkillSlot::HidTipWidget()
{
	SetVisibilityTipWidget(false);
}

void UUI_SkillSlot::ShowTipWidget()
{
	SetVisibilityTipWidget(true);
}

bool UUI_SkillSlot::IsText() const
{
	if (UUI_Tip* InTip = Cast<UUI_Tip>(ToolTipWidget.Get())) {
		return InTip->IsText();
	}
	return false;
}
#pragma endregion 关于Tip悬浮框的一些接口

// 控制条件型技能的 技能槽UI数据(在一段时长内)
void UUI_SkillSlot::UpdateConditionalSkillSlot(float InStartPos, float InEndPos)
{
	GThread::Get()->GetCoroutines().BindLambda(InStartPos, [&]() {
		bWaitUntil = true;
		UUI_SlotElement::SlotIcon->SetIsEnabled(true);
		});

	GThread::Get()->GetCoroutines().BindLambda(InEndPos, [&]() {
		bWaitUntil = false;
		UUI_SlotElement::SlotIcon->SetIsEnabled(false);
		});
}

// 设定技能槽内的GA
void UUI_SkillSlot::UpdateGameplayAbility(UMMOARPGGameplayAbility* InGameplayAbility)
{
	SlotInfo.GameplayAbility = InGameplayAbility;
}

#undef LOCTEXT_NAMESPACE
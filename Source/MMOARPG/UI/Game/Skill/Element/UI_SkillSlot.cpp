#include "UI_SkillSlot.h"
#include "Components/Button.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "DragDrop/UI_ICODragDrog.h"
#include "Styling/SlateBrush.h"
#include "../../../../Core/Game/Character/MMOARPGCharacter.h"

int32 UUI_SkillSlot::PlayerSkillNumber = 0;

UUI_SkillSlot::UUI_SkillSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bMappingKey(false)
	, KeyNumber(INDEX_NONE)
{

}

void UUI_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (bMappingKey) {
		//映射键位
		KeyNumber = ++PlayerSkillNumber;

		// 屏幕打印按下了哪个键位
		FString PlayerSkillName = FString::Printf(TEXT("PlayerSkill_%i"), PlayerSkillNumber);
		
		// 键鼠输入绑定回调
		GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(*PlayerSkillName, IE_Pressed, this, &UUI_SkillSlot::OnClickedWidget);
		GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(*PlayerSkillName, IE_Released, this, &UUI_Slot::OnReleasedClickedWidget);

		//设置Key名称
		SetKeyName(FString::FromInt(PlayerSkillNumber));

		// 暂定5个键位, 超出了则重置映射键位
		if (PlayerSkillNumber >= 5) {
			PlayerSkillNumber = 0;
		}
	}
}

void UUI_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_SkillSlot::OnClickedWidget()
{
	UUI_Base::Print(FString::FromInt(KeyNumber));

	if (KeyNumber > 0) {
		if (AMMOARPGCharacter* InCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacter>()) {
			if (InCharacter->GetActionState() == ECharacterActionState::FIGHT_STATE) {// 仅当进入战斗行为状态
				// 服务端执行技能形式的技能攻击(需指定一个槽号)
				InCharacter->SKillAttackOnServer(KeyNumber);
			}
		}
	}
}

void UUI_SkillSlot::OnReleasedClickedWidget()
{

}

#pragma region 对外接口
void UUI_SkillSlot::Update(const FName& InTagName, UTexture2D* InTexture)
{
	this->SetIcon(InTexture);
	this->SlotInfo.Tags = InTagName;// 注册技能名字为入参.
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

				/** I. 技能插槽交换行为 */
				if (MyInventorySlot->GetSlotInfo().IsVaild() && this->GetSlotInfo().IsVaild()) {// 拖拽出来的插槽技能信息和 自己本身的插槽技能信息 名字都有意义
					/* 1.1 服务器表现 */
					{
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
					/* 1.2 客户端表现的效果.*/
					{
						// 技能效果
						{
							// 先缓存 "右键拖拽Logo"的纹理和名字
							UTexture2D* TmpTexture = MyInventorySlot->GetIcon();
							FName TmpTags = MyInventorySlot->GetSlotInfo().Tags;

							// 给 "右键拖拽Logo" 重新写入
							MyInventorySlot->SetIcon(GetIcon());
							MyInventorySlot->GetSlotInfo().Tags = GetSlotInfo().Tags;

							// 将自己的技能插槽信息更换为之前缓存的那一份
							GetSlotInfo().Tags = TmpTags;
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
				/** II. 技能插槽移动; // 拖拽出来的有技能, 自己作为接收方没有技能,这种行为称之为技能移动 */
				else if (MyInventorySlot->GetSlotInfo().IsVaild() && !this->GetSlotInfo().IsVaild()) {
					/* 2.1 服务器表现 */
					{
						if (!MyInventorySlot->IsSkillTableSlot() && !IsSkillTableSlot()) {// 双方都在技能框内
							// 通知服务端 这2个槽号的槽移动
							InCharacter->SKillSlotMoveToNewSlot(MyInventorySlot->KeyNumber, KeyNumber);
						}
						else if (MyInventorySlot->IsSkillTableSlot() && !IsSkillTableSlot()) {// 对方是从技能页拖出来的,而自己是在技能框里的
							//从SkillTable里面移动过来一个技能到空的技能表里面
							InCharacter->SKillTableSlotMoveToSkillSlot(MyInventorySlot->GetSlotInfo().Tags, KeyNumber);
						}
						else if (!MyInventorySlot->IsSkillTableSlot() && IsSkillTableSlot()) {// 对方是技能框,自己是技能页
							InCharacter->SKillSlotMoveToSkillTable(MyInventorySlot->KeyNumber);/** 从横框到技能页: 移动 */
						}
					}
					/* 2.2 客户端表现 */
					{
						// 技能效果
						{
							// 设置一下自身
							this->GetSlotInfo().Tags = MyInventorySlot->GetSlotInfo().Tags;
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
				/** III. 其他行为(比如拖拽失败了) */
				else {
					// 让拖拽Logo在过程中显示出来
					MyInventorySlot->VisibleIcon();
				}
				return true;
			}
		}
	}

	return bDrop;
}
#pragma endregion 覆写侦测拖拽手势系统函数
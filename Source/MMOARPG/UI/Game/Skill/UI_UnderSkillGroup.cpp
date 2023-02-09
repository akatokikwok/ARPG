#include "UI_UnderSkillGroup.h"
#include "Blueprint/DragDropOperation.h"
#include "../../../Core/Game/MMOARPGGameState.h"
#include "Components/HorizontalBox.h"
#include "Element/UI_SkillSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "../../../Core/Game/MMOARPGPlayerController.h"
#include "../../../Core/Game/Character/Core/MMOARPGCharacterBase.h"
#include "MMOARPG/Core/Game/Abilities/MMOARPGGameplayAbility.h"
#include "../../../Core/Game/MMOARPGHUD.h"

void UUI_UnderSkillGroup::NativeConstruct()
{
	Super::NativeConstruct();
	//LayoutSlot(TArray<FName>());

	if (AMMOARPGPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AMMOARPGPlayerController>()) {
		// 为委托"更新技能节点(横框内SkillSlots)" 注册回调
		InPlayerController->UpdateSkillSlotDelegate.BindUObject(this, &UUI_UnderSkillGroup::UpdateSkillSlots);

		// 为委托"更新技能CD"注册UI上的回调
		InPlayerController->UpdateSkillCooldownDelegate.BindUObject(this, &UUI_UnderSkillGroup::UpdateSkillCD);
	}

	// 屏蔽输入的计时代理
	bShieldSkill.Fun.BindLambda([&]() {
		ShieldSkillInput(true);
		});

	/** 在技能页里有2个HUD的公共事件(用来联系技能页与技能横框), 绑定回调 */
	if (AMMOARPGHUD* InMMOARPGHUD = UUI_Base::GetHUD<AMMOARPGHUD>()) {
		InMMOARPGHUD->BorderHeightDisplayDelegate.AddUObject(this, &UUI_UnderSkillGroup::SetBorderHeight);
		InMMOARPGHUD->ResetHeightDisplayDelegate.AddUObject(this, &UUI_UnderSkillGroup::ResetBorderHeight);
	}
}

void UUI_UnderSkillGroup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	bShieldSkill.Tick(InDeltaTime);
}

/** InSkillTags一共有十个技能,使用它们对技能横框初始化 */
void UUI_UnderSkillGroup::LayoutSlot(const TArray<FName>& InSkillTags)
{
	if (SkillSlotClass/*!SlotArray->GetChildrenCount() && */) {
		if (AMMOARPGCharacterBase* InCharacterBase = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacterBase>()) {
			if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
				TArray<FCharacterSkillTable*> SkillTables;// 主角身上的所有技能
				int32 CharacterID = InCharacterBase->GetID();
				if (InGameState->GetCharacterSkillsTables(CharacterID, SkillTables)) {// 能提取出主角身上的所有技能

					UpdateSlot(InSkillTags, SkillTables, SlotArrayA, 0, 5, InCharacterBase->GetCharacterLevel());
					UpdateSlot(InSkillTags, SkillTables, SlotArrayB, 5, 6, InCharacterBase->GetCharacterLevel());
				}
			}
		}
	}
}

/**
 * 接口: 更新技能横框
 */
void UUI_UnderSkillGroup::UpdateSlot(
	const TArray<FName>& InSkillTags,// 传入的一组技能
	TArray<FCharacterSkillTable*> InSkillTables,// 挑选出来的合适技能行
	UHorizontalBox* InSlotArray,// 哪组横框
	int32 InStart,// 一组技能中开始解算的起始序号
	int32 AppendSize,// 长度
	int32 InCharacterLevel)// 玩家GAS等级
{
	// Lambda-刷新某个skillslot所有数据和外观
	auto UpdateSlotWidgetLambda = [&](int32 InMyRow, UUI_SkillSlot* InSkillSlotWidget) ->void {
		if (InSkillTags.IsValidIndex(InMyRow)) {
			// 检索所有 匹配的技能行
			for (auto& SkillTmp : InSkillTables) {
				if (UMMOARPGGameplayAbility* InMMOGA = Cast<UMMOARPGGameplayAbility>(SkillTmp->GameplayAbility->GetDefaultObject())) {
					if (InSkillTags[InMyRow].ToString() == InMMOGA->AbilityTags.ToStringSimple()) {// 找出名字一致的那一行
						// 刷新这个skillslot
						InSkillSlotWidget->Update(InSkillTags[InMyRow], SkillTmp->Icon, InMMOGA->CostValue("Mana", InCharacterLevel), SkillTmp->SkillType);
						InSkillSlotWidget->SetTipTextContent(SkillTmp->SkillAttributeTip);
						break;
					}
				}
			}
		}
	};

	if (InSlotArray->GetChildrenCount() == 0) {/// 表明是重新创建技能组
		int32 RowNumber = InStart + AppendSize;
		for (int32 MyRow = InStart; MyRow < RowNumber; MyRow++) {
			if (UUI_SkillSlot* SlotWidget = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {
				// 横框设定为skillslot的父级面板
				SlotWidget->SetParents(this);

				//
				if (UHorizontalBoxSlot* BoxSlot = InSlotArray->AddChildToHorizontalBox(SlotWidget)) {
					BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				}

				// 刷新某个skillslot所有数据和外观
				UpdateSlotWidgetLambda(MyRow, SlotWidget);
			}
		}
	}
	else {/// 表明是刷新或重新更新技能组
		int32 Index = InStart;
		CallSKillSlot(
			InSlotArray,
			[&](UUI_SkillSlot* InSkillSlot) ->bool {
				if (InSkillSlot) {
					InSkillSlot->ResetTipText();
					
					if (InSkillTags.IsValidIndex(Index)) {
						/* 不相等则表明 客户端模拟的不正确,需由服务器去矫正 */
						if (InSkillSlot->GetSlotInfo().Tags != InSkillTags[Index]) {
							// 先清除原先的图标
							InSkillSlot->ResetIcon();
							InSkillSlot->GetSlotInfo().Reset();

							// 刷新某个skillslot所有数据和外观
							UpdateSlotWidgetLambda(Index, InSkillSlot);
						}
					}
				}
				Index++;// 计数迭代
				return false;
			});
	}
}

void UUI_UnderSkillGroup::UpdateSkillSlots(const TArray<FName>& InSkillTags)
{
	LayoutSlot(InSkillTags);
}

void UUI_UnderSkillGroup::UpdateSkillCD(const FName& InTagName, float InCDValue)
{
	auto JudgeMethod = [&](UUI_SkillSlot* InSkillSlot) ->bool {
		if (InSkillSlot) {
			if (InSkillSlot->GetSlotInfo().Tags == InTagName) {
				InSkillSlot->StartUpdateCD(InCDValue);
				InSkillSlot->SetMaxCD(InCDValue);
				return true;
			}
		}
		return false;
	};

	CallSKillSlot(SlotArrayA, JudgeMethod);
	CallSKillSlot(SlotArrayB, JudgeMethod);

	// 建议屏蔽技能输入CD一秒
	ShieldSkillInput(1.0f);
}

// 使用传入的lambda专门处理技能横框的特定slot
void UUI_UnderSkillGroup::CallSKillSlot(UHorizontalBox* InSlotBox, TFunction<bool(UUI_SkillSlot*)> InLambda)
{
	if (InSlotBox) {
		for (int32 i = 0; i < InSlotBox->GetChildrenCount(); ++i) {
			if (InLambda(Cast<UUI_SkillSlot>(InSlotBox->GetChildAt(i)))) {
				break;
			}
		}
	}
}

// 在一段时间内 屏蔽技能输入
void UUI_UnderSkillGroup::ShieldSkillInput(float InTime)
{
	ShieldSkillInput(false);

	bShieldSkill = true;
	bShieldSkill = InTime;
}

// 是否禁用技能输入
void UUI_UnderSkillGroup::ShieldSkillInput(bool bShield)
{
	auto HandleMethod = [&](UUI_SkillSlot* InSkillSlot) ->bool {
		if (InSkillSlot) {
			InSkillSlot->SetIsEnabled(bShield);
		}
		return false;
	};

	CallSKillSlot(SlotArrayA, HandleMethod);
	CallSKillSlot(SlotArrayB, HandleMethod);
}

void UUI_UnderSkillGroup::SetBorderHeight(EMMOARPGSkillType InSkillType)
{
	auto TmpHandle = [&](UUI_SkillSlot* InSkillSlot) {
		if (InSkillSlot) {
			if (InSkillSlot->GetSkillType() == InSkillType ||
				InSkillSlot->GetSkillType() == EMMOARPGSkillType::NONE_SKILLS) {
				InSkillSlot->SetVisibilityBorderHeight(true);
				InSkillSlot->SetIsEnabled(true);
			}
			else {
				InSkillSlot->SetVisibilityBorderHeight(false);
				//InSkillSlot->SetIsEnabled(false);
			}
		}

		return false;
	};

	CallSKillSlot(SlotArrayA, TmpHandle);
	CallSKillSlot(SlotArrayB, TmpHandle);
}

// 处理高亮的接口, 复位高亮
void UUI_UnderSkillGroup::ResetBorderHeight()
{
	auto TmpLambda = [&](UUI_SkillSlot* InSkillSlot) {
		if (InSkillSlot) {
			InSkillSlot->SetVisibilityBorderHeight(false);
			InSkillSlot->SetIsEnabled(true);
		}
		return false;
	};

	CallSKillSlot(SlotArrayA, TmpLambda);
	CallSKillSlot(SlotArrayB, TmpLambda);
}

﻿#include "UI_UnderSkillGroup.h"
#include "Blueprint/DragDropOperation.h"
#include "../../../Core/Game/MMOARPGGameState.h"
#include "Components/HorizontalBox.h"
#include "Element/UI_SkillSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "../../../Core/Game/MMOARPGPlayerController.h"
#include "../../../Core/Game/Character/Core/MMOARPGCharacterBase.h"
#include "MMOARPG/Core/Game/Abilities/MMOARPGGameplayAbility.h"

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
}

void UUI_UnderSkillGroup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	bShieldSkill.Tick(InDeltaTime);
}

void UUI_UnderSkillGroup::LayoutSlot(const TArray<FName>& InSkillTags)
{
	if (SlotArray->GetChildrenCount() == 0) {// 表明是重新创建技能组

		if (/*!SlotArray->GetChildrenCount() && */SkillSlotClass) {
			if (AMMOARPGCharacterBase* InCharacterBase = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacterBase>()) {
				if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {

					TArray<FCharacterSkillTable*> SkillTables;
					int32 CharacterID = InCharacterBase->GetID();
					if (InGameState->GetCharacterSkillsTables(CharacterID, SkillTables)) {

						int32 RowNumber = 5;
						for (int32 MyRow = 0; MyRow < RowNumber; MyRow++) {
							if (UUI_SkillSlot* SlotWidget = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {
								//
								SlotWidget->SetParents(this);

								//
								if (UHorizontalBoxSlot* BoxSlot = SlotArray->AddChildToHorizontalBox(SlotWidget)) {
									BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
									BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
									BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
								}

								// 更新具体数据和技能
								if (InSkillTags.IsValidIndex(MyRow)) {
									// 检索所有 匹配的技能行
									for (auto& SkillTmp : SkillTables) {
										if (UMMOARPGGameplayAbility* InMMOGA = Cast<UMMOARPGGameplayAbility>(SkillTmp->GameplayAbility->GetDefaultObject())) {
											if (InSkillTags[MyRow].ToString() == InMMOGA->AbilityTags.ToStringSimple()) {// 找出名字一致的那一行
												// 刷新这个skillslot
												SlotWidget->Update(InSkillTags[MyRow], 
													SkillTmp->Icon, 
													InMMOGA->CostValue("Mana", InCharacterBase->GetCharacterLevel()));
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else {// 表明是刷新或重新更新技能组

		int32 Index = 0;
		CallSKillSlot([&](UUI_SkillSlot* InSkillSlot) ->bool {
			if (InSkillSlot) {
				if (InSkillTags.IsValidIndex(Index)) {
					/* 不相等则表明 客户端模拟的不正确,需由服务器去矫正 */
					if (InSkillSlot->GetSlotInfo().Tags != InSkillTags[Index]) {
						if (AMMOARPGCharacterBase* InCharacterBase = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacterBase>()) {
							if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
								TArray<FCharacterSkillTable*> SkillTables;
								int32 CharacterID = InCharacterBase->GetID();
								if (InGameState->GetCharacterSkillsTables(CharacterID, SkillTables)) {
									// 先清除原先的图标
									InSkillSlot->ResetIcon();
									InSkillSlot->GetSlotInfo().Reset();

									// 先查找到和入参匹配的技能行
									if (FCharacterSkillTable** MySkillTableRow = SkillTables.FindByPredicate(
										[&](FCharacterSkillTable* InTableRowInfo)->bool {
											if (UGameplayAbility* InGA = Cast<UGameplayAbility>(InTableRowInfo->GameplayAbility->GetDefaultObject())) {
												if (InSkillTags[Index].ToString() == InGA->AbilityTags.ToStringSimple()) {
													return true;
												}
											}
											return false;
										})) {
										// 这个技能槽去 更新 上一步符合条件的技能行里的图标
										InSkillSlot->Update(InSkillTags[Index], (*MySkillTableRow)->Icon);
									}
								}
							}
						}
					}
					//
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
	CallSKillSlot([&](UUI_SkillSlot* InSkillSlot) ->bool {
		if (InSkillSlot) {
			if (InSkillSlot->GetSlotInfo().Tags == InTagName) {
				InSkillSlot->StartUpdateCD(InCDValue);
				InSkillSlot->SetMaxCD(InCDValue);
				return true;
			}
		}
		return false;
		}
	);

	// 建议屏蔽技能输入CD一秒
	ShieldSkillInput(1.0f);
}

// 使用传入的lambda专门处理技能横框的特定slot
void UUI_UnderSkillGroup::CallSKillSlot(TFunction<bool(UUI_SkillSlot*)> InLambda)
{
	if (SlotArray) {
		for (int32 i = 0; i < SlotArray->GetChildrenCount(); ++i) {
			if (InLambda(Cast<UUI_SkillSlot>(SlotArray->GetChildAt(i)))) {
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
	CallSKillSlot([&](UUI_SkillSlot* InSkillSlot) ->bool {
		if (InSkillSlot) {
			InSkillSlot->SetIsEnabled(bShield);
		}
		return false;
		});
}

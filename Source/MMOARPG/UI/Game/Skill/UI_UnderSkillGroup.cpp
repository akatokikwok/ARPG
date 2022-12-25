#include "UI_UnderSkillGroup.h"
#include "Blueprint/DragDropOperation.h"
#include "../../../Core/Game/MMOARPGGameState.h"
#include "Components/HorizontalBox.h"
#include "Element/UI_SkillSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "../../../Core/Game/MMOARPGPlayerController.h"
#include "../../../Core/Game/Character/Core/MMOARPGCharacterBase.h"

void UUI_UnderSkillGroup::NativeConstruct()
{
	Super::NativeConstruct();
	//LayoutSlot(TArray<FName>());

	if (AMMOARPGPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AMMOARPGPlayerController>()) {
		// 为委托"更新技能节点(横框内SkillSlots)" 注册回调
		InPlayerController->UpdateSkillSlotDelegate.BindUObject(this, &UUI_UnderSkillGroup::UpdateSkillSlots);
	}
}

void UUI_UnderSkillGroup::LayoutSlot(const TArray<FName>& InSkillTags)
{
	SlotArray->ClearChildren();

	if (/*!SlotArray->GetChildrenCount() && */SkillSlotClass) {
		if (AMMOARPGCharacterBase* InCharacterBase = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacterBase>()) {
			if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {

				TArray<FCharacterSkillTable*> SkillTables;
				int32 CharacterID = InCharacterBase->GetID();
				if (InGameState->GetCharacterSkillsTables(CharacterID, SkillTables)) {

					int32 RowNumber = 5;
					for (int32 MyRow = 0; MyRow < RowNumber; MyRow++) {
						if (UUI_SkillSlot* SlotWidget = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {
							if (UHorizontalBoxSlot* BoxSlot = SlotArray->AddChildToHorizontalBox(SlotWidget)) {
								BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
								BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
								BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
							}

							// 更新具体数据和技能
							if (InSkillTags.IsValidIndex(MyRow)) {
								// 先查找到和入参匹配的技能行
								if (FCharacterSkillTable** MySkillTableRow = SkillTables.FindByPredicate(
									[&](FCharacterSkillTable* InTableRowInfo)->bool {
										if (UGameplayAbility* InGA = Cast<UGameplayAbility>(InTableRowInfo->GameplayAbility->GetDefaultObject())) {
											if (InSkillTags[MyRow].ToString() == InGA->AbilityTags.ToStringSimple()) {
												return true;
											}
										}
										return false;
									})) {
									// 这个技能槽去 更新 上一步符合条件的技能行里的图标
									SlotWidget->Update(InSkillTags[MyRow], (*MySkillTableRow)->Icon);
								}
							}
						}
					}
				}
			}
		}
	}
}

void UUI_UnderSkillGroup::UpdateSkillSlots(const TArray<FName>& InSkillTags)
{
	LayoutSlot(InSkillTags);
}
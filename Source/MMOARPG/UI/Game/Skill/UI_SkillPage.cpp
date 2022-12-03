#include "UI_SkillPage.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Blueprint/DragDropOperation.h"
#include "../../../Core/Game/MMOARPGGameState.h"
#include "../../../Core/Game/Character/Core/MMOARPGCharacterBase.h"
#include "Abilities/GameplayAbility.h"
#include "Element/UI_SkillSlot.h"
#include "../../../Core/Game/MMOARPGPlayerController.h"

void UUI_SkillPage::NativeConstruct()
{
	Super::NativeConstruct();
	LayoutSlot(TArray<FName>());
}

void UUI_SkillPage::LayoutSlot(const TArray<FName>& InKeys)
{
	if (SlotArrayInventory->GetChildrenCount() == 0 && SkillSlotClass != nullptr) {
		if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
			int32 RowNumber = 20;
			int32 ColumNumber = 3;
			for (int32 MyRow = 0; MyRow < RowNumber; MyRow++) {
				for (int32 MyColum = 0; MyColum < ColumNumber; MyColum++) {
					if (UUI_SkillSlot* SlotWidget = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {
						if (UUniformGridSlot* GridSlot = SlotArrayInventory->AddChildToUniformGrid(SlotWidget)) {
							GridSlot->SetColumn(MyColum);
							GridSlot->SetRow(MyRow);
							GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
							GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
						}
					}
				}
			}
		}
	}
}

void UUI_SkillPage::UpdateSkillTable(const TArray<FName>& InSkillTags)
{

}
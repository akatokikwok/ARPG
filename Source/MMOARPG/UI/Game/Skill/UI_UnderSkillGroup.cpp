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
	LayoutSlot(TArray<FName>());

	if (AMMOARPGPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AMMOARPGPlayerController>()) {
		// 为委托"更新技能节点(横框内SkillSlots)" 注册回调
		InPlayerController->UpdateSkillSlotDelegate.BindUObject(this, &UUI_UnderSkillGroup::UpdateSkillSlots);
	}
}

void UUI_UnderSkillGroup::LayoutSlot(const TArray<FName>& InSkillTags)
{
	if (!SlotArray->GetChildrenCount() && SkillSlotClass) {
		if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
			int32 RowNumber = 5;
			for (int32 MyRow = 0; MyRow < RowNumber; MyRow++) {
				if (UUI_SkillSlot* SlotWidget = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {
					if (UHorizontalBoxSlot* BoxSlot = SlotArray->AddChildToHorizontalBox(SlotWidget)) {
						BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
						BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
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
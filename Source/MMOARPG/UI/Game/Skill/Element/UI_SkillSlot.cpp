#include "UI_SkillSlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "DragDrop/UI_ICODragDrog.h"
#include "Styling/SlateBrush.h"
#include "../../../../Core/Game/Character/MMOARPGCharacter.h"

void UUI_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_SkillSlot::OnClickedWidget()
{

}

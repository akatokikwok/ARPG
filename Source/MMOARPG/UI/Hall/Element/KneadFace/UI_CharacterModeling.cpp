#include "UI_CharacterModeling.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "../../../../Core/Hall/HallPlayerState.h"
#include "../../../../Core/Hall/HallPawn.h"
#include "../../../../Core/Hall/Character/CharacterStage.h"

void UUI_CharacterModeling::NativeConstruct()
{
	Super::NativeConstruct();

	LegSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::LegValueChanged);
	WaistSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::WaistValueChanged);
	ArmSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::ArmValueChanged);
	ModelingType->OnSelectionChanged.AddDynamic(this, &UUI_CharacterModeling::SelectModelingType);
}

void UUI_CharacterModeling::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterModeling::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_CharacterModeling::LegValueChanged(float InDeltaTime)
{

}

void UUI_CharacterModeling::WaistValueChanged(float InDeltaTime)
{

}

void UUI_CharacterModeling::ArmValueChanged(float InDeltaTime)
{

}

void UUI_CharacterModeling::SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

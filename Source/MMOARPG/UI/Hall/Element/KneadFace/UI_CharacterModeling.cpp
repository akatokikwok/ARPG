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

/** 刷新舞台人物身材长相. */
void UUI_CharacterModeling::UpdatePawn()
{
	if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
		if (InPawn->CharacterStage != nullptr) {
			InPawn->CharacterStage->UpdateKneadingBoby();
		}
	}
}

/** 设定滑块条右侧的数字文字. */
void UUI_CharacterModeling::UpdateText(UTextBlock* InValueText, float InValue)
{
	if (InValueText) {
		// 以此种算法设计出诸如00 01 02 03 04的数字.
		int32 IntValue = InValue * 10;
		InValueText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), IntValue)));
	}
}

void UUI_CharacterModeling::LegValueChanged(float InVal)
{
	UpdateText(LegValueText, InVal);// 设定滑块条右侧的数字文字.
	UpdatePawn();// 更新舞台人物外观.
}

void UUI_CharacterModeling::WaistValueChanged(float InVal)
{
	UpdateText(WaistValueText, InVal);
	UpdatePawn();
}

void UUI_CharacterModeling::ArmValueChanged(float InVal)
{
	UpdateText(ArmValueText, InVal);
	UpdatePawn();
}

void UUI_CharacterModeling::SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

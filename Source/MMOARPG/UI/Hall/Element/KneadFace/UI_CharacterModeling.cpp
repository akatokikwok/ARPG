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
	ChestSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::ChestValueChanged);
	HeadSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::HeadValueChanged);

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
		if (AHallPlayerState* InState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>()) {
			if (InPawn->CharacterStage) {
				int32 SlotID = InPawn->CharacterStage->GetSlotID();
				FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotID);
				if (!InCA) {
					if (auto* CurrentTmpCreateCharacter = InState->GetCurrentTmpCreateCharacterCA()) {
						if (CurrentTmpCreateCharacter->SlotPosition == SlotID) {
							InCA = CurrentTmpCreateCharacter;
						}
					}
				}

				if (InCA) //有更多参数
				{
					InCA->LegSize = LegSlider->GetValue() * 10.f;
					InCA->WaistSize = WaistSlider->GetValue() * 10.f;
					InCA->ArmSize = ArmSlider->GetValue() * 10.f;
					InCA->HeadSize = HeadSlider->GetValue() * 10.f;
					InCA->ChestSize = ChestSlider->GetValue() * 10.f;
				}

				InPawn->CharacterStage->UpdateKneadingBoby();
			}
		}
	}

	// 	if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
// 		if (AHallPlayerState* InPS = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>())
// 		{
// 			if (InPawn->CharacterStage != nullptr) {
// 				int32 SlotID_Stagechar = InPawn->CharacterStage->GetSlotID();// 1.拿取舞台人物槽号.
// 				FMMOARPGCharacterAppearance* InCA_Stage = InPS->GetCharacterCA(SlotID_Stagechar);// 2.在PS里找对应槽号的CA存档.
// 				
// 				// 比对临时的CA 匹配 舞台人物槽号, 查无此CA则暗示这是新建的舞台人物.
// 				if (!InCA_Stage) {
// 					if (auto* CA_CurrentTmpCreateCharacter = InPS->GetCurrentTmpCreateCharacterCA()) {
// 						if (CA_CurrentTmpCreateCharacter->SlotPosition == SlotID_Stagechar) {
// 							InCA_Stage = CA_CurrentTmpCreateCharacter;
// 						}
// 					}
// 				}
// 
// 				// 刷新CA存档数据为滑条对应传出的值.
// 				if (InCA_Stage) {
// 					InCA_Stage->LegSize = LegSlider->GetValue() * 10.f;
// 					InCA_Stage->WaistSize = WaistSlider->GetValue() * 10.f;
// 					InCA_Stage->ArmSize = ArmSlider->GetValue() * 10.f;
// 					InCA_Stage->HeadSize = HeadSlider->GetValue() * 10.f;
// 					InCA_Stage->ChestSize = ChestSlider->GetValue() * 10.f;
// 				}
// 
// 				// 更新外观
// 				InPawn->CharacterStage->UpdateKneadingBoby();
// 			}
// 		}
// 	}

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

void UUI_CharacterModeling::HeadValueChanged(float InVal)
{
	UpdateText(HeadValueText, InVal);
	UpdatePawn();
}

void UUI_CharacterModeling::ChestValueChanged(float InVal)
{
	UpdateText(ChestValueText, InVal);
	UpdatePawn();
}

void UUI_CharacterModeling::SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

void UUI_CharacterModeling::InitKneadFace(const FMMOARPGCharacterAppearance* InACData)
{
	if (InACData) {
		auto GetKFValue = [](float InValue)->float {
			return InValue / 10.f;
		};

		//更新值
		LegSlider->SetValue(GetKFValue(InACData->LegSize));
		WaistSlider->SetValue(GetKFValue(InACData->WaistSize));
		ArmSlider->SetValue(GetKFValue(InACData->ArmSize));
		HeadSlider->SetValue(GetKFValue(InACData->HeadSize));
		ChestSlider->SetValue(GetKFValue(InACData->ChestSize));

		//更新显示的值
		LegValueChanged(GetKFValue(InACData->LegSize));
		WaistValueChanged(GetKFValue(InACData->WaistSize));
		ArmValueChanged(GetKFValue(InACData->ArmSize));
		ChestValueChanged(GetKFValue(InACData->ChestSize));
		HeadValueChanged(GetKFValue(InACData->HeadSize));
	}
}

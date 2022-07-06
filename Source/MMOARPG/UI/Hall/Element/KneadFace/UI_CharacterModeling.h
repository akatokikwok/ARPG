#pragma once

#include "CoreMinimal.h"
#include "Core/UI_KneadFaceBase.h"
#include "UI_CharacterModeling.generated.h"

class UComboBoxString;
class USlider;
class UTextBlock;

UCLASS()
class MMOARPG_API UUI_CharacterModeling : public UUI_KneadFaceBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/** 通用接口:刷新舞台人物身材长相. */
	void UpdatePawn();
	/** 通用接口: 设定滑块条右侧的数字文字 */
	void UpdateText(UTextBlock* InValueText, float InValue);

private:
	//
	UFUNCTION()
		void LegValueChanged(float InVal);
	//
	UFUNCTION()
		void WaistValueChanged(float InVal);
	//
	UFUNCTION()
		void ArmValueChanged(float InVal);
	//
	UFUNCTION()
		void SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType);

private:
	UPROPERTY(meta = (BindWidget))
		UComboBoxString* ModelingType;

	UPROPERTY(meta = (BindWidget))
		USlider* LegSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* LegValueText;

	UPROPERTY(meta = (BindWidget))
		USlider* WaistSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* WaistValueText;

	UPROPERTY(meta = (BindWidget))
		USlider* ArmSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ArmValueText;
};
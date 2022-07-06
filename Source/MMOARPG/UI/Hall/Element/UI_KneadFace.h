// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../UI/Core/UI_Base.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"
#include "MMOARPGType.h"
#include "UI_KneadFace.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_KneadFace : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	// UCheckBox-Body被敲击了之后
	UFUNCTION()
		void OnClickedBody(bool bClicked);
	// UCheckBox-Appearance被敲击了之后
	UFUNCTION()
		void OnClickedAppearance(bool bClicked);
	// UCheckBox-Talenty被敲击了之后
	UFUNCTION()
		void OnClickedTalent(bool bClicked);

private:
 	UPROPERTY(meta = (BindWidget))
 		UCheckBox* Body;
 
 	UPROPERTY(meta = (BindWidget))
 		UCheckBox* Appearance;
 
 	UPROPERTY(meta = (BindWidget))
 		UCheckBox* Talent;
 
 	UPROPERTY(meta = (BindWidget))
 		UWidgetSwitcher* FacePanel;
public:
	//
	void InitKneadFace(const FMMOARPGCharacterAppearance* InACData);
};

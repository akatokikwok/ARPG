// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_MainBase.h"
#include "UI_PartnerInfo.generated.h"

class UCheckBox;
class UImage;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_PartnerInfo : public UUI_Base
{
	GENERATED_BODY()
private:
	friend class UUI_PartnerList;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* ClickedCheckBox;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBar;

public:
	UUI_PartnerInfo(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	// 回调; 当点了CheckBox.
	UFUNCTION()
	void OnClickedCharacter(bool bClicked);
	// 拿人物信息控件哪个实例的ID号.
	int32 GetCharacterID() { return CharacterID; }
	// 设定本CheckBox是否处于选中外观.
	void ShowSelected(bool bShow);
protected:
	// 设置人物信息控件的ID号.
	void SetCharacterID(int32 InCharacterID) { CharacterID = InCharacterID; }
	
protected:
	int32 CharacterID;// 第几号人物.
};

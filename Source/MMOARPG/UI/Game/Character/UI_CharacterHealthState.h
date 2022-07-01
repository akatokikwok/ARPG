// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_HealthState.h"
#include "UI_CharacterHealthState.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 玩家的血条Widget
 */
UCLASS()
class MMOARPG_API UUI_CharacterHealthState : public UUI_HealthState
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ManaText;
public:
	void SetMana(float InPercentage);
	void SetManaValue(float InMaxValue, float InCurrentValue);
	void SetHealthValue(float InMaxValue, float InCurrentValue);
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};

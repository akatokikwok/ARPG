// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Skill/Element/DragDrop/UI_DragDrog.h"
#include "UI_HealthState.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 最简易的血条widget,仅携带等级与血条进度.(如野怪, 老怪)
 */
UCLASS()
class MMOARPG_API UUI_HealthState : public UUI_DragDrog
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterLevel;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

public:

	void SetLv(int32 InNewLv);
	void SetHealth(float InPercentage);
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};

// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_HealthState.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#define LOCTEXT_NAMESPACE "UUI_HealthState"

void UUI_HealthState::SetLv(int32 InNewLv)
{
	CharacterLevel->SetText(FText::Format(LOCTEXT("HealthState_SetLv", "Lv {0}"), FText::FromString(FString::FromInt(InNewLv))));
}

void UUI_HealthState::SetHealth(float InPercentage)
{
	HealthBar->SetPercent(InPercentage);
}

void UUI_HealthState::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_HealthState::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_HealthState::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

#undef LOCTEXT_NAMESPACE
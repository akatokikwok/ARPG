// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_CharacterHealthState.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#define LOCTEXT_NAMESPACE "UUI_CharacterHealthState"

void UUI_CharacterHealthState::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_CharacterHealthState::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterHealthState::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_CharacterHealthState::SetMana(float InPercentage)
{
	ManaBar->SetPercent(InPercentage);
}

void UUI_CharacterHealthState::SetManaValue(float InMaxValue, float InCurrentValue)
{
	ManaText->SetText(FText::Format(LOCTEXT("UI_CharacterHealthState_SetManaValue", "{0} / {1}"), (int32)InCurrentValue, (int32)InMaxValue));
}

void UUI_CharacterHealthState::SetHealthValue(float InMaxValue, float InCurrentValue)
{
	HealthText->SetText(FText::Format(LOCTEXT("UI_CharacterHealthState_SetHealthValue", "{0} / {1}"), (int32)InCurrentValue, (int32)InMaxValue));
}

#undef LOCTEXT_NAMESPACE
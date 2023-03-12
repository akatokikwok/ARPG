// Fill out your copyright notice in the Description page of Project Settings.
#include "ExecutionCalculation_DaytonFrame.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "AbilitySystemComponent.h"

UExecutionCalculation_DaytonFrame::UExecutionCalculation_DaytonFrame()
	:DaytonFrameDuration(0.1f)
{

}

void UExecutionCalculation_DaytonFrame::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetAC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetAC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(SourceAC->GetOwner())) {
			if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
				if (DaytonFrameDuration > 0.001f) {
					InCharacter->DaytonFrame(DaytonFrameDuration);
				}
			}
		}
	}
}
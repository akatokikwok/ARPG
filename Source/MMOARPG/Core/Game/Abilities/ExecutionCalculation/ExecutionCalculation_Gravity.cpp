// Fill out your copyright notice in the Description page of Project Settings.
#include "ExecutionCalculation_Gravity.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "AbilitySystemComponent.h"

UExecutionCalculation_Gravity::UExecutionCalculation_Gravity()
	: DelayTime(1.0f)
{

}

void UExecutionCalculation_Gravity::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetAC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAC = ExecutionParams.GetSourceAbilitySystemComponent();

	// 让自己处于无重力滞空一会儿
	if (TargetAC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(SourceAC->GetOwner())) {
			if (DelayTime > 0.03f) {
				InCharacter->EnableGravityMulticast(DelayTime);
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecutionCalculation_Execute_GE.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

UExecutionCalculation_Execute_GE::UExecutionCalculation_Execute_GE()
	:bTaget(true)// 默认用在目标身上
{

}

void UExecutionCalculation_Execute_GE::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	if (!GameplayEffect) {
		return;
	}

	UAbilitySystemComponent* ASC = NULL;
	if (bTaget) {
		ASC = ExecutionParams.GetTargetAbilitySystemComponent();
	}
	else {
		ASC = ExecutionParams.GetSourceAbilitySystemComponent();
	}
	/* 仅在服务端勒令授予的buff效果在其挂载的ASC上生效 */
	if (ASC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(ASC->GetOwner())) {
			if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
				InCharacter->ExecuteGameplayEffect(GameplayEffect);
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.
#include "ExecutionCalculation_Execute_GA.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "AbilitySystemComponent.h"

UExecutionCalculation_Execute_GA::UExecutionCalculation_Execute_GA()
	: bTaget(true)// 默认用在目标身上
	, MMOARPGGameplayAbilityType(EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_NONE)
{

}

void UExecutionCalculation_Execute_GA::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	if (MMOARPGGameplayAbilityType != EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_NONE) {
		UAbilitySystemComponent* ASC = NULL;

		// 应用在目标/敌人身上
		if (bTaget) {
			ASC = ExecutionParams.GetTargetAbilitySystemComponent();
		}
		else {
			ASC = ExecutionParams.GetSourceAbilitySystemComponent();
		}
		/* 仅在服务端应用蓝图里配置好的GA(含技能来源和技能GA)*/
		if (ASC) {
			if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(ASC->GetOwner())) {
				if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
					InCharacter->ExecuteGameplayAbility(MMOARPGGameplayAbilityType, ExecuteTag.GetTagName());
				}
			}
		}
	}
}
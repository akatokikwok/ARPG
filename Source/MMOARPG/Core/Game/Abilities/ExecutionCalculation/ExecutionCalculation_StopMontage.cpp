// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecutionCalculation_StopMontage.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "AbilitySystemComponent.h"

UExecutionCalculation_StopMontage::UExecutionCalculation_StopMontage()
{

}

void UExecutionCalculation_StopMontage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	// 获取到自己的ASC和目标的ASC
	UAbilitySystemComponent* TargetAC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetAC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(SourceAC->GetOwner())) {
			if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {// 由于碰撞体检测就执行在服务器上,所以这里蒙太奇冲刺撞击也推荐只承认服务端
				InCharacter->StopAnimMontageOnMulticast();
				InCharacter->AnimSignal(2);
			}
		}
	}
}
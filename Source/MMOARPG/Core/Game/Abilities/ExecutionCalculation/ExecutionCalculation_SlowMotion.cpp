// Fill out your copyright notice in the Description page of Project Settings.
#include "ExecutionCalculation_SlowMotion.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

UExecutionCalculation_SlowMotion::UExecutionCalculation_SlowMotion()
	: Super()
	, Duration(1.f)
	, Speed(0.2f)
{

}

void UExecutionCalculation_SlowMotion::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetAC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAC = ExecutionParams.GetSourceAbilitySystemComponent();

	/** 这里是SourceAC, 即改变自己的子弹时间,把自己时间放慢 */
	if (SourceAC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(SourceAC->GetOwner())) {
			if (!InCharacter->IsDie()) {
				// 仅在客户端实现 存续一段时长的时间膨胀慢动作.
				InCharacter->PlaySlowMotionOnClient(Duration, Speed);
			}
		}
	}
}
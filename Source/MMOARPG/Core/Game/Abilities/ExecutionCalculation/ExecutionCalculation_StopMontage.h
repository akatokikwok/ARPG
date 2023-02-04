// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_StopMontage.generated.h"

/**
 * 一个Buff内配置的ExecutionCalculation,即GEEC
 * 用于停止播放蒙太奇
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_StopMontage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecutionCalculation_StopMontage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

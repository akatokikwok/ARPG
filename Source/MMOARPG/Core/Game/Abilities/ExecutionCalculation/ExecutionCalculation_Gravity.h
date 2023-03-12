// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_Gravity.generated.h"

/**
 * GEEC, 可以让人在滞空时候停留一会儿
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_Gravity : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecutionCalculation_Gravity();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:
	// 无重力滞空的持续时长
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ResetHitID)
		float DelayTime;
};

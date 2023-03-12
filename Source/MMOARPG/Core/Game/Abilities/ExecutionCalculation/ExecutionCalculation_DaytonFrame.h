// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_DaytonFrame.generated.h"

/**
 * 顿帧GEEC.
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_DaytonFrame : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	/** 顿帧持续时长 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayMontage")
		float DaytonFrameDuration;
public:
	UExecutionCalculation_DaytonFrame();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

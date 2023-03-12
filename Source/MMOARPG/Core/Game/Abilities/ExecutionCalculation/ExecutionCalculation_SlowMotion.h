// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_SlowMotion.generated.h"

/**
 * 慢动作/子弹时间的GEEC.
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_SlowMotion : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecutionCalculation_SlowMotion();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:
	// 效果持续时长
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlowMotion")
		float Duration;

	// 慢速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlowMotion")
		float Speed;
};

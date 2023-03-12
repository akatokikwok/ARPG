// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_Execute_GE.generated.h"

class UGameplayEffect;

/**
 * GEEC: 负责释放技能中会授予某个对象1个Buff
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_Execute_GE : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecutionCalculation_Execute_GE();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:
	// 蓝图可配置 是使用在自己身上还是敌人身上 勾选表示应用在目标身上
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Execute_GE)
		bool bTaget;

	// 要授予哪个buff
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Execute_GE)
		TSubclassOf<UGameplayEffect> GameplayEffect;
};

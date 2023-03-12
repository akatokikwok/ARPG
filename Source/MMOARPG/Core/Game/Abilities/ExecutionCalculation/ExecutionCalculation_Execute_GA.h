// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "../../../../MMOARPGGameType.h"
#include "ExecutionCalculation_Execute_GA.generated.h"

struct FGameplayTag;

/**
 * GEEC: 负责释放技能中会授予某个对象1个GA
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_Execute_GA : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecutionCalculation_Execute_GA();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:
	// 蓝图可配置 是使用在自己身上还是敌人身上 勾选表示应用在目标身上
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilityType)
		bool bTaget;

	// 蓝图可配置的 欲授予的技能来源
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilityType)
		EMMOARPGGameplayAbilityType MMOARPGGameplayAbilityType;

	// 由蓝图配置的 欲授予的 GA名字.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayAbilityType)
		FGameplayTag ExecuteTag;
};

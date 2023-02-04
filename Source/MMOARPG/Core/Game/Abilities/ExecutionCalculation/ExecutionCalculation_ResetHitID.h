// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_ResetHitID.generated.h"

/**
 * 一个GEEC,负责重置敌人的受击ID
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_ResetHitID : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecutionCalculation_ResetHitID();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:
	// 授予敌人 空中的受击ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ResetHitID")
		int32 HitIDByGround;

	// 授予敌人 地面的受击ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ResetHitID")
		int32 HitIDByAir;

	// 是否启用 敌人激活受击技能(bHit)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ResetHitID")
		bool bAuthorityPlayHit;
};

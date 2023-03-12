#pragma once
#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_ZeroVelocity.generated.h"

/**
 * 1个GEEC,负责瞬时 冻结人物的速度
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_ZeroVelocity : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecutionCalculation_ZeroVelocity();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "NumericalAlgorithmExecuteObject.generated.h"

/**
 * 数值推演算法的对象
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLENUMERICALDEDUCTION_API UNumericalAlgorithmExecuteObject : public UObject
{
	GENERATED_BODY()
public:
	virtual float GetAlgorithmValue() { return 0.f; }
};
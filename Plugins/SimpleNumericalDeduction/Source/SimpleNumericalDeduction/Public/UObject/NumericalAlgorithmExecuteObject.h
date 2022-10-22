#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "NumericalAlgorithmExecuteObject.generated.h"
struct FNAEParam;

/**
 * 数值推演算法的对象
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLENUMERICALDEDUCTION_API UNumericalAlgorithmExecuteObject : public UObject
{
	GENERATED_BODY()
public:
	/** 传进来一个参数包, 依据策划配好的算法来对该参数包推导出一个构想值 */
	virtual float GetAlgorithmValue(const FNAEParam& InParam);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "AlgorithmValue", meta = (DisplayName = "GetFloatAlgorithmValue"))
		float K2_GetAlgorithmValue(const FNAEParam& InParam);
	float K2_GetAlgorithmValue_Implementation(const FNAEParam& InParam) { return 0.f; };
};
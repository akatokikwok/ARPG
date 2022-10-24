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
	virtual float GetFloatAlgorithmValue(const FNAEParam& InParam);
	virtual int32 GetInt32AlgorithmValue(const FNAEParam& InParam);
	virtual FString GetStringAlgorithmValue(const FNAEParam& InParam);

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "AlgorithmValue", meta = (DisplayName = "GetFloatAlgorithmValue"))
		float K2_GetAlgorithmValue_Float(const FNAEParam& InParam);
	float K2_GetAlgorithmValue_Float_Implementation(const FNAEParam& InParam) { return 0.f; };

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "AlgorithmValue", meta = (DisplayName = "GetInt32AlgorithmValue"))
		int32 K2_GetAlgorithmValue_Int32(const FNAEParam& InParam);
	int32 K2_GetAlgorithmValue_Int32_Implementation(const FNAEParam& InParam) { return 0; };

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "AlgorithmValue", meta = (DisplayName = "GetStringAlgorithmValue"))
		FString K2_GetAlgorithmValue_String(const FNAEParam& InParam);
	FString K2_GetAlgorithmValue_String_Implementation(const FNAEParam& InParam) { return TEXT(""); };
};
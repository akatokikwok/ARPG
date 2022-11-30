#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "NumericalAlgorithmExecuteObject.generated.h"
struct FNAEParam;

/**
 * 数值推演算法的对象
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLENUMERICALDEDUCTIONRUNTIME_API UNumericalAlgorithmExecuteObject : public UObject
{
	GENERATED_BODY()
public:
	/** 传进来一个参数包, 依据策划配好的算法来对该参数包推导出一个构想值 */
	virtual float GetFloatAlgorithmValue(const FNAEParam& InParam);
	virtual int32 GetInt32AlgorithmValue(const FNAEParam& InParam);
	virtual FString GetStringAlgorithmValue(const FNAEParam& InParam);

	/** 计算主动玩家对被动玩家的伤害值 */
	virtual float GetDamageAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData);

	/** 计算主动玩家对被动玩家的治疗值 */
	virtual float GetTreatmentAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData);

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

	// 伤害
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "AlgorithmValue", meta = (DisplayName = "GetDamageAlgorithmValue"))
		float K2_GetDamageAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData);
	float K2_GetDamageAlgorithmValue_Implementation(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData) { return 0.f; };

	// 治疗
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "AlgorithmValue", meta = (DisplayName = "GetTreatmentAlgorithmValue"))
		float K2_GetTreatmentAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData);
	float K2_GetTreatmentAlgorithmValue_Implementation(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData) { return 0.f; };
};
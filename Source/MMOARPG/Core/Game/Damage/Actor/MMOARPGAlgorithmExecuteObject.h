#pragma once

#include "CoreMinimal.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"
#include "MMOARPGAlgorithmExecuteObject.generated.h"

/**
 * MMO自己的玩家对玩家的数值算法; 
 * 继承自插件里的玩家对玩家的数值算法
 */
UCLASS(BlueprintType, Blueprintable)
class UMMOARPGAlgorithmExecuteObject : public UNumericalAlgorithmExecuteObject
{
	GENERATED_BODY()
public:
	UMMOARPGAlgorithmExecuteObject();

	virtual float GetFloatAlgorithmValue(const FNAEParam& InParam) override;
	virtual int32 GetInt32AlgorithmValue(const FNAEParam& InParam) override;
	virtual float GetDamageAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData) override;
	virtual float GetTreatmentAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData) override;
};
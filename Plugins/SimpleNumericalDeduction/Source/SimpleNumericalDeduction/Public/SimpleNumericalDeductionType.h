#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.generated.h"
class UNumericalAlgorithmExecuteObject;

/**
 * 推演属性元数据
 */
USTRUCT(BlueprintType)
struct FDeduceAttributeData
{
	GENERATED_USTRUCT_BODY()
public:

public:
	// 代表LV 蓝量 血量 攻击诸如此类的Key字段.
	UPROPERTY()
		FName Key;
	// 可配置的系数.
	UPROPERTY()
		float Coefficient;
	// 推演算法.
	UPROPERTY()
		TSubclassOf<UNumericalAlgorithmExecuteObject> BaseAlgorithm;
};

/**
 * 承载 "推演属性元数据"的表
 */
USTRUCT(BlueprintType)
struct FDeduceAttributeDataTables
{
	GENERATED_USTRUCT_BODY()
public:
	// 推演属性 表名.
	UPROPERTY()
		FName TableName;
	// 多个推演元数据.
	UPROPERTY()
		TArray<FDeduceAttributeData> AttributeDatas;
};
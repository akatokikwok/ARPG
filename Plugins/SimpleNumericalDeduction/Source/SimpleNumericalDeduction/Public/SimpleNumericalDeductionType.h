#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.generated.h"
class UNumericalAlgorithmExecuteObject;

/**
 * 数值算法object会用到的一个参数包结构体.
 * 表示 "NAE基础属性"
 */
USTRUCT(BlueprintType)
struct SIMPLENUMERICALDEDUCTION_API FNAEParam
{
	GENERATED_USTRUCT_BODY()
public:
	// 代表现归属于哪个attribute
	UPROPERTY(BlueprintReadOnly)
		FString Key;

	// 经过指定算法推导(可能有多次迭代)出来的推导值.
	UPROPERTY(BlueprintReadOnly)
		float Value;

	// 迭代次数
	UPROPERTY(BlueprintReadOnly)
		int32 Count;// 迭代次数

	// 推导系数
	UPROPERTY(BlueprintReadOnly)
		float Coefficient;// 推导系数
	
	// <单属性(如蓝, 血), 该属性值>
	UPROPERTY(BlueprintReadOnly)
		TMap<FString, FString> AttributeData;// <单属性(如蓝, 血), 该属性值>
};

/**
 * 单属性数据
 * 推演属性元数据
 */
USTRUCT(BlueprintType)
struct FDeduceAttributeData
{
	GENERATED_USTRUCT_BODY()
public:
	FDeduceAttributeData();

public:
	// 单个属性内, 代表LV 蓝量 血量 攻击诸如此类的Key字段.
	UPROPERTY()
		FName Key;// 单个属性内, 代表LV 蓝量 血量 攻击诸如此类的Key字段.

	// 代表各属性具体的值 (如血量还剩15).
	UPROPERTY()
		FString Value;// 代表各属性具体的值 (如血量还剩15).

	// 可配置的系数.
	UPROPERTY()
		float Coefficient;// 可配置的系数.
	
	// 承载推导出来的浮点
	UPROPERTY()
		TArray<FString> DeduceValue;// 承载推导出来的浮点

	// 推演算法.
	UPROPERTY()
		TSubclassOf<UNumericalAlgorithmExecuteObject> BaseAlgorithm;// 推演算法.
};

/**
 * 多属性结构 (持有多条单个具体属性) 
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

	// 所有的 单个具体属性 合集
	UPROPERTY()
		TArray<FDeduceAttributeData> AttributeDatas;// 一个包含所有单属性的数组.
};
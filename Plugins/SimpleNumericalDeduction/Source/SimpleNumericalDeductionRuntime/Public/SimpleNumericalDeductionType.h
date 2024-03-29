﻿#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.generated.h"
class UNumericalAlgorithmExecuteObject;

/**
 * 预打印日志行
 */
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FSimplePreDebugPrintf
{
	FString CharacterNameActive;
	FString CharacterNamePassive;
	FString EventString;
	FString Value;
};

/**
 * 管理预打印日志的类
 */
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FDebugPrintfLogContainer
{
public:
	void Add(
		const FString& InCharacterNameActive,
		const FString& InCharacterNamePassive,
		const FString& InEventString,
		const FString& InValue);

	void Pop(FSimplePreDebugPrintf& Out);

	bool IsPop();

private:
	TArray<FSimplePreDebugPrintf> Logs;
};

// SIMPLENUMERICALDEDUCTIONRUNTIME_API FDebugPrintfLogContainer LogContainer;

/**
 * 欲被反射的变量数据结构
 */
USTRUCT(BlueprintType)
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FSimpleSelectString
{
	GENERATED_USTRUCT_BODY()
public:
	FSimpleSelectString();

public:
	// 需要被写入的Key(表名)
	UPROPERTY(EditAnywhere)
		FString SelectString;// 需要被写入的Key(表名)

	// 下拉菜单的string
	UPROPERTY(EditAnywhere)
		TArray<FString> SelectStrings;// 下拉菜单的string
};

/**
 * 描述日志信息关于玩家等级的打印策略
 * N指向哪个玩家,哪个玩家等级将失效
 */
UENUM(BlueprintType)
enum class EIterativeDebugPrintMethod :uint8
{
	ONE_TO_MANY_METHOD		UMETA(DisplayName = "1->N"),// 1对多
	MANY_TO_MANY_METHOD		UMETA(DisplayName = "N->N"),// 同步升级
	MANY_TO_ONCE_METHOD		UMETA(DisplayName = "N->1"),// 多对1
};

/**
 * 区分玩家对玩家的行为(如造成伤害、治疗)
 */
UENUM(BlueprintType)
enum class EActionCharacterEventType : uint8
{
	DAMAGE_EVENT		UMETA(DisplayName = "Damage"),
	TREATMENT_EVENT		UMETA(DisplayName = "Treatment"),
};

/**
 * 调试日志关联到的玩家信息
 */
USTRUCT(BlueprintType)
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FDebugCharacterInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FDebugCharacterInfo();

	// 欲被反射的定义变量
	UPROPERTY(EditAnywhere)
		FSimpleSelectString Key;// 欲被反射的定义变量,表名

	// 玩家等级, 不勾选bIterationCount 才会显示本迭代次数
	UPROPERTY(EditAnywhere)
		int32 Level;// 玩家等级, 不勾选bIterationCount 才会显示本迭代次数
};

/**
 *  调试日志关联到的 多个玩家之间的信息
 */
USTRUCT(BlueprintType)
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FDebugCharactersInfo
{
	GENERATED_USTRUCT_BODY()
public:
	// 测试用的 玩家对战算法
	UPROPERTY(EditAnywhere)
		TSubclassOf<UNumericalAlgorithmExecuteObject> TestAlgorithmObject;// 测试用的 玩家对战算法

	// 主动
	UPROPERTY(EditAnywhere)
		TArray<FDebugCharacterInfo> CharacterActive;

	// 玩家对玩家的行为
	UPROPERTY(EditAnywhere)
		EActionCharacterEventType EventType;

	// 被动
	UPROPERTY(EditAnywhere)
		TArray<FDebugCharacterInfo> CharacterPassive;
};

/**
 * 推演数据的类型
 */
UENUM(BlueprintType)
enum class EDeduceAttributeDataType :uint8
{
	DEDUCETYPE_INT32  UMETA(DisplayName = "in32"),
	DEDUCETYPE_FLOAT  UMETA(DisplayName = "float"),
	DEDUCETYPE_STRING UMETA(DisplayName = "string"),
};

/**
 * 数值算法object会用到的一个参数包结构体.
 * 表示 "NAE基础属性"
 */
USTRUCT(BlueprintType)
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FNAEParam
{
	GENERATED_USTRUCT_BODY()
public:
	// 代表现归属于哪个attribute
	UPROPERTY(BlueprintReadOnly)
		FString Key;// 代表现归属于哪个attribute

	// 经过指定算法推导(可能有多次迭代)出来的推导值.
	UPROPERTY(BlueprintReadOnly)
		float Value;// 经过指定算法推导(可能有多次迭代)出来的推导值.

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
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FDeduceAttributeData
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

	// 推演数值是什么类型?(浮点, 字符)
	UPROPERTY()
		EDeduceAttributeDataType AttributeDataType;// 推演数值是什么类型?(浮点, 字符)

	// 推演算法.
	UPROPERTY()
		TSubclassOf<UNumericalAlgorithmExecuteObject> BaseAlgorithm;// 推演算法.
};

/**
 * 多属性结构 (持有多条单个具体属性)
 * 承载 "推演属性元数据"的表
 */
USTRUCT(BlueprintType)
struct SIMPLENUMERICALDEDUCTIONRUNTIME_API FDeduceAttributeDataTables
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
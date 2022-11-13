#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "SNDObjectSettings.generated.h"
class UDataTable;
class UNumericalAlgorithmExecuteObject;

/**
 * 推演用的SND对象
 */
UCLASS(config = SNDObjectSettings)
class SIMPLENUMERICALDEDUCTIONRUNTIME_API USNDObjectSettings : public UObject
{
	GENERATED_BODY()
public:
	USNDObjectSettings();

public:
	// 推演的基础表
	UPROPERTY(EditAnywhere, Category = "DeductionSettings", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		UDataTable* BaseTable;// 推演的基础表

	// CSV存储的位置
	UPROPERTY(config, EditAnywhere, Category = "DeductionSettings", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		FDirectoryPath CSVSavePath;// CSV存储的位置

	// 推演的次数
	UPROPERTY(config, EditAnywhere, Category = "DeductionSettings", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		int32 DeductionNumber;// 推演的次数

	// 多个表的集合; 存放被生成的具体属性数据; UPROPERTY(config)表示直接生成的不可被编辑.
	UPROPERTY(config, VisibleAnywhere, Category = "InvisibleData", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		TArray<FDeduceAttributeDataTables> AttributeDatas;// 总数据, 是一个表群

	// 总控制的那个算法Obj,和算法模板只能二选一
	UPROPERTY(config, EditAnywhere, Category = "AlgorithmExecuteObject", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		TSubclassOf<UNumericalAlgorithmExecuteObject> NumericalAlgorithmExecuteObject;// 总控制的那个算法Obj,和算法模板只能二选一

 	// 算法模板; UPROPERTY(config)表示直接生成的不可被编辑.
 	UPROPERTY(config, EditAnywhere, Category = "AlgorithmExecuteObject", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
 		TArray<TSubclassOf<UNumericalAlgorithmExecuteObject>> NumericalAlgorithmExecuteObjects;// 算法模板

	// 总控制系数
	UPROPERTY(config, EditAnywhere, Category = "Coefficient", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		float Coefficient;// 总控制系数

	// 各自的系数
	UPROPERTY(config, EditAnywhere, Category = "Coefficient", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		TArray<float> Coefficients;// 各自的系数

public:
	/** 解析基础表 */
	bool AnalysisBaseTable();
	
	// 保存slate样式上的SND数据至本地磁盘.
	void SaveObjectConfig();
	// 读取保存在本地磁盘的"slate样式上的SND数据"
	void LoadObjectConfig();

private:
	// 返回一个路径, 1个指定位置的 "slate样式上的SND数据"
	FString GetDefaultConfigFilename();
};
﻿#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "..\UObject\NumericalAlgorithmExecuteObject.h"
#include "SNDObjectSettings.generated.h"
class UDataTable;

/**
 * 推演用的SND对象
 */
UCLASS(config = SNDObjectSettings)
class USNDObjectSettings : public UObject
{
	GENERATED_BODY()
public:
	USNDObjectSettings();

public:
	// 推演的基础表
	UPROPERTY(EditAnywhere, Category = "DeductionSettings", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		UDataTable* BaseTable;

	// CSV存储的位置
	UPROPERTY(config, EditAnywhere, Category = DeductionSettings, meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		FDirectoryPath CSVSavePath;

	// 推演的次数
	UPROPERTY(config, EditAnywhere, Category = "DeductionSettings", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		int32 DeductionNumber;

	// 多个表的集合; 存放被生成的具体属性数据; UPROPERTY(config)表示直接生成的不可被编辑.
	UPROPERTY(config, VisibleAnywhere, Category = "DeductionSettings", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		TArray<FDeduceAttributeDataTables> AttributeDatas;// 总数据, 是一个表群

 	// 算法模板; UPROPERTY(config)表示直接生成的不可被编辑.
 	UPROPERTY(config, EditAnywhere, Category = "DeductionSettings", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
 		TArray<TSubclassOf<UNumericalAlgorithmExecuteObject>> NumericalAlgorithmExecuteObjects;// 算法模板

public:
	/** 解析基础表 */
	bool AnalysisBaseTable();

};
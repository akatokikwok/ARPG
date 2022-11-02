#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "Widgets/SCompoundWidget.h"
class SVerticalBox;

/**
 * 玩家基础属性编辑器控件, 派生自SCompoundWidget.
 */
class SSDataTableAttributeTable :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSDataTableAttributeTable)
	{}
	SLATE_END_ARGS()
public:
	/** Slate控件的渲染入口都从Construct开始. */
	void Construct(const FArguments& InArgs, FDeduceAttributeDataTables& InDeduceAttributeDataTables);
public:
	/** 生成1个新的曲线编辑器 */
	FReply SpawnNewAttributeCurveTable();
	/** 拿取表名给button用 */
	FText GetTableName() const;

private:
	//
	TSharedPtr<SVerticalBox> ListVerticalBox;
	// 多属性数据; 承载 "推演属性元数据"的表
	FDeduceAttributeDataTables* AttributeDataTablesPtr;
};
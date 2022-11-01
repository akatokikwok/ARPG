#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
class SCurveEditorPanel;
class FCurveEditor;

/**
 * 弹出的曲线窗口table
 */
class FSDeduceAttributeCurveTable
{
public:
	FSDeduceAttributeCurveTable();

public:
	/** 在本模块启动必须先调用的函数; 先注册本窗口 */
	void InitLayout();

	//
	void Construct(FDeduceAttributeDataTables& InDeduceAttributeDataTables);
	//
	void Construct(FDeduceAttributeData& InDeduceAttributeDataTable);

	/** 帮助开发者生成曲线标签页 */
	TSharedRef<SDockTab> SpawnTab_CurveAsset(const FSpawnTabArgs& Args);

private:
	TSharedPtr<FCurveEditor> CurveEditor;
	TSharedPtr<SCurveEditorPanel> CurveEditorPanel;

	// 多属性
	FDeduceAttributeDataTables* DeduceAttributeDataTables;
	// 单属性
	FDeduceAttributeData* DeduceAttributeDataTable;
};
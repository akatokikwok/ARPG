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
	/** 在本模块启动必须先调用的函数; 先注册本窗口纯UI布局 */
	void InitLayout();

	// 注册曲线编辑器数据 并启动标签页
	void Construct(FDeduceAttributeDataTables& InDeduceAttributeDataTables);
	//
	void Construct(FDeduceAttributeData& InDeduceAttributeDataTable);

	/** 帮助开发者生成曲线标签页 */
	TSharedRef<SDockTab> SpawnTab_CurveAsset(const FSpawnTabArgs& Args);

private:
	// 一个UE的曲线编辑器
	TSharedPtr<FCurveEditor> CurveEditor;
	// 一个UE的曲线编辑器panel
	TSharedPtr<SCurveEditorPanel> CurveEditorPanel;

	// 多属性数据源
	FDeduceAttributeDataTables* DeduceAttributeDataTables;
	// 单属性数据源
	FDeduceAttributeData* DeduceAttributeDataTable;
};
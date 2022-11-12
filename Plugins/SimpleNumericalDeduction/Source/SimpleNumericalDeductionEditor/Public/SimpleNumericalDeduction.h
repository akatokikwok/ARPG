// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Widget/Editor/DeduceAttributeCurveTable.h"
#include "SimpleNumericalDeductionType.h"
#include "Widget/Editor/DebugAttributeDeduce.h"

class FToolBarBuilder;
class FMenuBuilder;

/**
 * 推演数值的模块
 */
class FSimpleNumericalDeductionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
public:
	/** 生成曲线编辑器table, 用多属性 */
	void SpawnAttributeCurveTable(FDeduceAttributeDataTables& InDeduceAttributeDataTables);
	
	/** 生成曲线编辑器table, 用单属性 */
	void SpawnAttributeCurveTable(FDeduceAttributeData& InDeduceAttributeDataTabl);

public:
	// 生成调试表
	void SpawnDebugDeductionTable();

private:
	void RegisterMenus();

	/** 标签页内生成一个数值推演控件 */
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	/** 注册当前自定义的数据对象  */
	void RegisterObjectCustomizations() {};
	/** 注册当前自定义的属性变量对象 */
	void RegisterPropertyTypeCustomizations();
	/** 注册单个变量用的方法 */
	void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	// 属性曲线Table
	FSDeduceAttributeCurveTable AttributeCurveTable;

	// 调试推演数据用的 视口
	FDebugAttibuteDeduce DebugAttributeDeduceTable;

private:
	// 存放注册变量的一个容器
	TSet<FName> RegisteredPropertyTypes;

};

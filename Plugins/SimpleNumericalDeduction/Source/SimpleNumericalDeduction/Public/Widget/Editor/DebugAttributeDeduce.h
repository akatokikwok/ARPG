#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"

/**
 * "负责调试属性"的元元素控件
 */
class FDebugAttibuteDeduce
{
public:
	FDebugAttibuteDeduce();
	// 初始化视口布局
	void InitLayout();
	// 构造调试用的视口
	void Construct();

public:
	/** 生成调试用的 标签页 */
	TSharedRef<SDockTab> SpawnTab_DebugSettingWidget(const FSpawnTabArgs& Args);

private:
	TSharedPtr<SWidget> WidgetBase;
};
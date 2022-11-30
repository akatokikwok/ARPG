#pragma once

#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 渲染在调试日志编辑器内部的 一行行日志
 */
class SAttributeLogBase :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAttributeLogBase)
	{}
	SLATE_END_ARGS()

public:
	// 构造器内需要一个预打印日志行
	void Construct(const FArguments& InArgs, const FSimplePreDebugPrintf& InMyLog);
};

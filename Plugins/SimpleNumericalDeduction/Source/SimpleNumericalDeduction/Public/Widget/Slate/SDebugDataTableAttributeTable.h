#pragma once

#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 调试日志编辑器
 */
class SDebugDataTableAttributeTable :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDebugDataTableAttributeTable)
	{}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	FReply SaveAsText();
	FReply ClearLog();
	FReply Generate();

public:

};
#pragma once

#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "Widgets/SCompoundWidget.h"

class SVerticalBox;

/**
 * 日志控件
 */
class SSimepleNumbericalDeductionLog : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSimepleNumbericalDeductionLog)
	{}
	SLATE_END_ARGS()

public:
	// 
	void Construct(const FArguments& InArgs);

public:
	// 
	void AddLog(const FString& InContent);
	//
	void AddLog(const FText& InContent);
	//
	void ClearLog();
	//
	void SaveAsText();

	//
	void Generate();

public:
	TSharedPtr<SVerticalBox> BoxList;
};
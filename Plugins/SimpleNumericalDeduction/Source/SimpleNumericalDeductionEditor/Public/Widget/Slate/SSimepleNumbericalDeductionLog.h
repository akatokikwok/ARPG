#pragma once

#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "Widgets/SCompoundWidget.h"
#include "Tickable.h"

class SVerticalBox;
struct FSimplePreDebugPrintf;

/**
 * 日志控件
 */
class SSimepleNumbericalDeductionLog : public SCompoundWidget, public FTickableGameObject
{
public:
	SLATE_BEGIN_ARGS(SSimepleNumbericalDeductionLog)
	{}
	SLATE_END_ARGS()

public:
	// 
	void Construct(const FArguments& InArgs);
	// 是否在编辑器下tick
	virtual bool IsTickableInEditor() const;
	//
	virtual void Tick(float DeltaTime) override;
	// 
	virtual TStatId GetStatId() const;

public:
	// 塞入1个预打印日志行
	void AddLog(const FSimplePreDebugPrintf& MyLog);

	//
	void ClearLog();
	//
	void SaveAsText();

	//
	void Generate();

public:
	TSharedPtr<SVerticalBox> BoxList;
};
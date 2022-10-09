#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 数值推演控件, 派生自SCompoundWidget.
 */
class SSimpleNumericalDeductionWidget :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSimpleNumericalDeductionWidget)
	{}
	SLATE_END_ARGS()
public:
	/** Slate控件的渲染入口都从Construct开始. */
	void Construct(const FArguments& InArgs);

public:
	// 接口: 储存为默认
	FReply SaveAsDefault();
	// 接口: 检验是否满足CSV条件
	bool IsEnableToCSV()const;
	// 接口: 储存为CSV
	FReply SaveAsCSV();

private:
	TSharedPtr<class SVerticalBox> VerticalList;
};
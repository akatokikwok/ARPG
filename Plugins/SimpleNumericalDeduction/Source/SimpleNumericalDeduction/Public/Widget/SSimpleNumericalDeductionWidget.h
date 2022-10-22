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
	// 接口: 生成推演
	FReply GenerateDeduction();
	// 接口: 是否满足生成推演
	bool IsGenerateDeduction() const;
	// 生成属性表
	FReply GenerateAttributeTable();
	// 是否满足生成属性表
	bool IsGenerateAttributeTable()const;

private:
	// 层层清除 所有推导值.
	void ClearDeductionValue();

	// 点击tab页内主面板上 Generate按钮 所生成推演的widget
	void GenerateDeductionWidget();

private:
	TSharedPtr<class SVerticalBox> VerticalList;
};
#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "Widgets/SCompoundWidget.h"
class SEditableTextBox;

/**
 * 扩展框点击按钮之后被生成出的控件
 */
class SSTableBaseAttribute :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSTableBaseAttribute)
	{}
	SLATE_END_ARGS()
public:
	/** 构造器, 需要传入1个"推演属性元数据" */
	void Construct(const FArguments& InArgs, FDeduceAttributeData& InDeduceAttributeDataTables);
public:
	/** 生成人物单属性的关联曲线表 */
	FReply SpawnSingleAttributeCurveTable();
	/** 当可配置的系数改变时刷新为新的配置系数; */
	void OnValueChanged(float InNewValue);
	/** 当某个Editbox被提交内容的时候 */
	void OnValueCommied(const FText& InText, ETextCommit::Type InType);

private:
	// "推演属性元数据"
	FDeduceAttributeData* AttributeDataTablesPtr;
	// 1个EditableBox
	TSharedPtr<SEditableTextBox> EditableTextBox;
};
#include "Widget/Slate/SDataTableAttributeTable.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Settings/SNDObjectSettings.h"
#include "SimpleNumericalDeduction.h"

#define LOCTEXT_NAMESPACE "SSDataTableAttributeTable"

void SSDataTableAttributeTable::Construct(const FArguments& InArgs, FDeduceAttributeDataTables& InDeduceAttributeDataTables)
{
	AttributeDataTablesPtr = &InDeduceAttributeDataTables;// 缓存入参的 推演表

	ChildSlot
	[
		/* 塞入一个扩展框*/
		SNew(SExpandableArea)
		
		.HeaderContent()// 定义扩展框的"头部"
		[
			SNew(SVerticalBox)
			
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.Text(this, &SSDataTableAttributeTable::GetTableName)
				.OnClicked(this, &SSDataTableAttributeTable::SpawnNewAttributeCurveTable)
				.ToolTipText(LOCTEXT("TableName_Tip", "Changes in the curve can be generated."))
			]
			+ SVerticalBox::Slot()// 放推演属性元数据下的所有字段,诸如Key, 配置系数, 推演算法
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Left).Padding(4.f, 2.f, 4.f, 2.f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SSDataTableAttributeTable_AttributeName", "Attribute Name"))
				]
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Left).Padding(4.f, 2.f, 4.f, 2.f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SSDataTableAttributeTable_AlgorithmCoefficient", "Coefficient"))
				]
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Left).Padding(4.f, 2.f, 4.f, 2.f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SSDataTableAttributeTable_CustomAlgorithm", "ActualCustomAlgorithm"))
				]
			]
		]
		.BodyContent()// 定义扩展框的"body"
		[
			SAssignNew(ListVerticalBox, SVerticalBox)
		]
	];
}

FReply SSDataTableAttributeTable::SpawnNewAttributeCurveTable()
{
	
	return FReply::Handled();
}

FText SSDataTableAttributeTable::GetTableName() const
{
	if (AttributeDataTablesPtr) {
		return FText::FromName(AttributeDataTablesPtr->TableName);
	}
	// 没有属性的话就给一个默认的本地化值
	return LOCTEXT("AttributeDataTables_TableName", "NONE_Attribute");
}

#undef LOCTEXT_NAMESPACE 
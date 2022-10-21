#include "Widget/Slate/SDataTableAttributeTable.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Settings/SNDObjectSettings.h"
#include "SimpleNumericalDeduction.h"
#include "Widget/Slate/SlateElement/STableBaseAttribute.h"
#include "SimpleNumericalDeductionType.h"

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
					.Text(LOCTEXT("SSDataTableAttributeTable_Value", "Value"))
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

	if (USNDObjectSettings* InSND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
		for (int32 i = 0; i < AttributeDataTablesPtr->AttributeDatas.Num(); i++) {
			
			/* 算法Obj: 都用总算法obj填充*/
			if (InSND->NumericalAlgorithmExecuteObject) {
				AttributeDataTablesPtr->AttributeDatas[i].BaseAlgorithm = InSND->NumericalAlgorithmExecuteObject;
			}
			else if (InSND->NumericalAlgorithmExecuteObjects.IsValidIndex(i)) {/* 算法Obj: 按各自的算法模板的位置执行匹配 */
				if (InSND->NumericalAlgorithmExecuteObjects[i]) {
					AttributeDataTablesPtr->AttributeDatas[i].BaseAlgorithm = InSND->NumericalAlgorithmExecuteObjects[i];
				}
			}

			// 使用共用的总系数
			if (InSND->Coefficient != 0.f) {
				AttributeDataTablesPtr->AttributeDatas[i].Coefficient = InSND->Coefficient;
			}
			else if (InSND->Coefficients.IsValidIndex(i))//系数: 按各自算法模板的位置进行匹配
			{
				if (InSND->Coefficients[i]) {
					AttributeDataTablesPtr->AttributeDatas[i].Coefficient = InSND->Coefficients[i];
				}
			}



			/* 生成并渲染子级别控件 */
			ListVerticalBox->AddSlot().AutoHeight()
			[
				SNew(SSTableBaseAttribute, AttributeDataTablesPtr->AttributeDatas[i])
			];
		}
	}
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
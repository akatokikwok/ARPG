#include "Widget/Slate/SlateElement/STableBaseAttribute.h"
#include "Widgets/Input/SSpinBox.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"
#include "SimpleNumericalDeduction.h"

void SSTableBaseAttribute::Construct(const FArguments& InArgs, FDeduceAttributeData& InDeduceAttributeDataTable)
{
	AttributeDataTablesPtr = &InDeduceAttributeDataTable;
	ChildSlot
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Fill).Padding(4.f, 2.f, 4.f, 2.f)
		[
			SNew(SButton)
			.Text(FText::FromName(InDeduceAttributeDataTable.Key))// 是对应的属性字段(如LV 血 蓝)
			.HAlign(HAlign_Fill)
			.OnClicked(this, &SSTableBaseAttribute::SpawnSingleAttributeCurveTable)
		]

		+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Left).Padding(4.f, 2.f, 4.f, 2.f)
		[
			SNew(SSpinBox<float>)
			.Value(InDeduceAttributeDataTable.Coefficient)// 可配置的系数
			.OnValueChanged(this, &SSTableBaseAttribute::OnValueChanged)
		]
	];
}

void SSTableBaseAttribute::OnValueChanged(float InNewValue)
{
	if (AttributeDataTablesPtr) {
		AttributeDataTablesPtr->Coefficient = InNewValue;
	}
}

void SSTableBaseAttribute::OnValueCommied(const FText& InText, ETextCommit::Type InType)
{
	if (AttributeDataTablesPtr) {
		if (UClass* InClass = StaticLoadClass(UNumericalAlgorithmExecuteObject::StaticClass(), NULL, *InText.ToString())) {
			AttributeDataTablesPtr->BaseAlgorithm = InClass;
		}
		else {
			AttributeDataTablesPtr->BaseAlgorithm = UNumericalAlgorithmExecuteObject::StaticClass();
		}
	}
}

FReply SSTableBaseAttribute::SpawnSingleAttributeCurveTable()
{

	return FReply::Handled();
}
#include "Widget/Slate/SlateElement/STableBaseAttribute.h"
#include "Widgets/Input/SSpinBox.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"
#include "SimpleNumericalDeduction.h"
#include "SimpleNumericalDeductionType.h"

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
			SNew(STextBlock)
			.Text(FText::FromString(InDeduceAttributeDataTable.Value))
		]

		+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Left).Padding(4.f, 2.f, 4.f, 2.f)
		[
			SNew(SSpinBox<float>)
			.Value(InDeduceAttributeDataTable.Coefficient)// 可配置的系数
			.OnValueChanged(this, &SSTableBaseAttribute::OnValueChanged)
		]

		+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Fill).Padding(4.f, 2.f, 4.f, 2.f)
		[
			SAssignNew(EditableTextBox, SEditableTextBox)
			.OnTextCommitted(this, &SSTableBaseAttribute::OnValueCommied)
		]
	];

	if (EditableTextBox && AttributeDataTablesPtr && AttributeDataTablesPtr->BaseAlgorithm) {
		/* 为其设置明确的算法内容 */
		// 切割并加工一下算法内容
		FString FullName = AttributeDataTablesPtr->BaseAlgorithm->GetFullName();
		FullName.RemoveFromStart(TEXT("Class "));
		FullName.RemoveFromStart(TEXT("BlueprintGeneratedClass "));
		// 传入并设置内容
		EditableTextBox->SetText(FText::FromString(FullName));
	}
}

/** 当可配置的系数改变时刷新为新的配置系数; */
void SSTableBaseAttribute::OnValueChanged(float InNewValue)
{
	if (AttributeDataTablesPtr) {
		AttributeDataTablesPtr->Coefficient = InNewValue;
	}
}

/** 当某个Editbox被提交内容的时候 */
void SSTableBaseAttribute::OnValueCommied(const FText& InText, ETextCommit::Type InType)
{
	if (AttributeDataTablesPtr) {
		if (UClass* InClass = StaticLoadClass(UNumericalAlgorithmExecuteObject::StaticClass(), nullptr, *InText.ToString())) {
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
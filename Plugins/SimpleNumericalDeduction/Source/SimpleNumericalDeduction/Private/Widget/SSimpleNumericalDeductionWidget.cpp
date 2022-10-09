#include "Widget/SSimpleNumericalDeductionWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "PropertyEditorModule.h"
#include "Settings/SNDObjectSettings.h"
#include "Widget/Slate/SDataTableAttributeTable.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"
#include "Misc/FileHelper.h"
#define LOCTEXT_NAMESPACE "SimpleNumericalDeductionWidget"// 定义本地化操作

void SSimpleNumericalDeductionWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SScrollBox)
		.Orientation(Orient_Vertical)
		+ SScrollBox::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[
				/* 垂直框里塞1个水平框.*/
				SNew(SHorizontalBox)
				// 先在靠右侧放1个按钮
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f)
				[
					SNew(SButton)
					.Text(LOCTEXT("SND_Save_as_Default", "Save as Default"))
					.HAlign(HAlign_Center)
					.OnClicked(this, &SSimpleNumericalDeductionWidget::SaveAsDefault)
					.ToolTipText(LOCTEXT("SND_Save_as_DefaultTip", "Store configuration locally."))
				]
				// 靠右侧再放1个按钮, 它管理CSV
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f).AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("SND_Save_as_csv", "Save as csv"))
					.HAlign(HAlign_Center)
					.IsEnabled(this, &SSimpleNumericalDeductionWidget::IsEnableToCSV)
					.OnClicked(this, &SSimpleNumericalDeductionWidget::SaveAsCSV)
					.ToolTipText(LOCTEXT("SND_Save_as_CSVTip", "This function is mainly used for calling gameplay system after exporting deduction results."))
				]

			]
		]
	];
}

FReply SSimpleNumericalDeductionWidget::SaveAsDefault()
{

	return FReply::Handled();
}

bool SSimpleNumericalDeductionWidget::IsEnableToCSV() const
{

	return false;
}

FReply SSimpleNumericalDeductionWidget::SaveAsCSV()
{

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE// 终止定义本地化操作
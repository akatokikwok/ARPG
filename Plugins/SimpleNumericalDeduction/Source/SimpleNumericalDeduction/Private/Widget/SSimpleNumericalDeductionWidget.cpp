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
	// 引入加载编辑器模块
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// 细节面板参数
	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowMultipleTopLevelObjects = true;// 指明是多个对象
		DetailsViewArgs.bShowPropertyMatrixButton = false;// 隐藏matrixbutton
	}
	// 创建1个细节面板
	TSharedPtr<class IDetailsView> ConfigPanel = PropertyModule.CreateDetailView(DetailsViewArgs);
	// 细节面板内塞入推演用的 SNDObject
	TArray<UObject*> SourceObjects;
	{
		SourceObjects.Add(const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>()));
	}
	ConfigPanel->SetObjects(SourceObjects);

	/* 具体内容*/
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
				// 先在靠右侧放1个按钮, 它管理Default
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
				//
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f).AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Generate_Attribute_Table", "Generate Attribute Table"))
					.HAlign(HAlign_Center)
					.IsEnabled(this, &SSimpleNumericalDeductionWidget::IsGenerateAttributeTable)
					.OnClicked(this, &SSimpleNumericalDeductionWidget::GenerateAttributeTable)
					.ToolTipText(LOCTEXT("Generate_Attribute_TableTip", "Generate Attribute data according to the table."))
				]
				// 塞入一个生成按钮
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f).AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Generate_Deduction", "Generate Deduction"))
					.HAlign(HAlign_Center)
					.IsEnabled(this, &SSimpleNumericalDeductionWidget::IsGenerateDeduction)
					.OnClicked(this, &SSimpleNumericalDeductionWidget::GenerateDeduction)
					.ToolTipText(LOCTEXT("Generate_DeductionTip", "Generate deduction data according to the table."))
				]
			]
			// 垂直框内塞入1个 细节面板
			+ SVerticalBox::Slot().AutoHeight()
			[
				ConfigPanel.ToSharedRef()
			]
			+ SVerticalBox::Slot().AutoHeight()
			[
				SAssignNew(VerticalList, SVerticalBox)
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

FReply SSimpleNumericalDeductionWidget::GenerateDeduction()
{
	// to do.

	return FReply::Handled();
}

bool SSimpleNumericalDeductionWidget::IsGenerateDeduction() const
{
	if (const USNDObjectSettings* SND = GetDefault<USNDObjectSettings>()) {
		if (!SND->AttributeDatas.IsEmpty() && SND->DeductionNumber >= 1) {
			return true;
		}
	}
	return false;
}

FReply SSimpleNumericalDeductionWidget::GenerateAttributeTable()
{
	if (VerticalList) {
		if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
			// 准备处理snd对象

			// 仅当SND对象内基础表被解析成功
			if (SND->AnalysisBaseTable()) {
				// 先清除所有子项.
				VerticalList->ClearChildren();

				// 扫描SND里总数据, 给每个表都生成1个垂直框(内嵌一个编辑器)
				for (auto& Tmp : SND->AttributeDatas) {
					VerticalList->AddSlot().AutoHeight()
					[
						SNew(SSDataTableAttributeTable, Tmp)
					];
				}
			}
		}
	}

	return FReply::Handled();
}

bool SSimpleNumericalDeductionWidget::IsGenerateAttributeTable() const
{
	if (const USNDObjectSettings* SND = GetDefault<USNDObjectSettings>()) {
		if (SND->BaseTable) {
			return true;
		}
	}

	return false;
}

#undef LOCTEXT_NAMESPACE// 终止定义本地化操作
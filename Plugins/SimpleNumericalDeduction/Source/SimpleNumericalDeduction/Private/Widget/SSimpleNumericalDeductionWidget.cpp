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

	if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
		// 读取 保存在本地路径下的Slate样式设置细节
		SND->LoadObjectConfig();
	}

	// 每次渲染本控件的时候亦会生成出上次保存好的推演控件
	this->GenerateDeductionWidget();
}

FReply SSimpleNumericalDeductionWidget::SaveAsDefault()
{
	if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
		// 保存 Slate样式设置细节 到本地
		SND->SaveObjectConfig();
	}
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

/** 生成推演 */
FReply SSimpleNumericalDeductionWidget::GenerateDeduction()
{
	// 先清除上一次产生的所有推导值
	ClearDeductionValue();

	if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) { // 拿取SND设置
		// 声明1个参数包
		FNAEParam NAEParam;

		/* 储存 由 两层for提出单个具体属性 至参数包的一个字段*/
		for (auto& MainTmp : SND->AttributeDatas) {
			for (auto& Tmp : MainTmp.AttributeDatas) {
				NAEParam.AttributeData.Add(Tmp.Key.ToString(), Tmp.Value);
			}
		}

		/* */
		for (auto& MainTmp : SND->AttributeDatas) {
			for (auto& Tmp_attri : MainTmp.AttributeDatas) {
				// 拿到单属性的算法obj
				if (Tmp_attri.BaseAlgorithm != nullptr) {
					if (UNumericalAlgorithmExecuteObject* InObject = Cast<UNumericalAlgorithmExecuteObject>(Tmp_attri.BaseAlgorithm->GetDefaultObject())) {
						
						NAEParam.Key = Tmp_attri.Key.ToString();
						Tmp_attri.DeduceValue.Add(Tmp_attri.Value);// 推导值先默认给一份

						// 推演 单属性
						for (int32 i = 0; i < SND->DeductionNumber; ++i) {
							NAEParam.Value = FCString::Atof(*Tmp_attri.DeduceValue.Last());// 用推导浮点集的最新浮点填充参数包
							NAEParam.Count = i + 2;
							NAEParam.Coefficient = Tmp_attri.Coefficient;
							float InValue = InObject->GetAlgorithmValue(NAEParam);
							// 更新单属性的 "被推导出的浮点"
							Tmp_attri.DeduceValue.Add(FString::SanitizeFloat(InValue));
						}
					}
				}
			}
		}
	}

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
	if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
		// 仅当SND对象内基础表被解析成功
		if (SND->AnalysisBaseTable()) {
			this->GenerateDeductionWidget();
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

void SSimpleNumericalDeductionWidget::ClearDeductionValue()
{
	/* 把所有单属性数据的 推导值置空 */
	if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {// 加载出SND设置
		for (auto& MainTmp : SND->AttributeDatas) {
			for (auto& Tmp : MainTmp.AttributeDatas) {
				Tmp.DeduceValue.Empty();
			}
		}
	}
}

void SSimpleNumericalDeductionWidget::GenerateDeductionWidget()
{
	if (VerticalList) {
		if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
			// 先清除所有子项.
			VerticalList->ClearChildren();

			// 扫描SND里总数据, 给每个单属性都生成1个垂直框(内嵌一个编辑器)
			for (auto& Tmp : SND->AttributeDatas) {
				VerticalList->AddSlot().AutoHeight()
				[
					SNew(SSDataTableAttributeTable, Tmp)
				];
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE// 终止定义本地化操作
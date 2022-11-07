#include "Widget/SSimpleNumericalDeductionWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "PropertyEditorModule.h"
#include "Settings/SNDObjectSettings.h"
#include "Widget/Slate/SDataTableAttributeTable.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"
#include "Misc/FileHelper.h"
#include "SimpleNumericalDeduction.h"
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
				// "根据诸SND设置生成属性表"按钮
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f).AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Generate_Attribute_Table", "Generate Attribute Table"))
					.HAlign(HAlign_Center)
					.IsEnabled(this, &SSimpleNumericalDeductionWidget::IsGenerateAttributeTable)
					.OnClicked(this, &SSimpleNumericalDeductionWidget::GenerateAttributeTable)
					.ToolTipText(LOCTEXT("Generate_Attribute_TableTip", "Generate Attribute data according to the table."))
				]
				//  "依据算法生成推演"按钮
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f).AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Generate_Deduction", "Generate Deduction"))
					.HAlign(HAlign_Center)
					.IsEnabled(this, &SSimpleNumericalDeductionWidget::IsGenerateDeduction)
					.OnClicked(this, &SSimpleNumericalDeductionWidget::GenerateDeduction)
					.ToolTipText(LOCTEXT("Generate_DeductionTip", "Generate deduction data according to the table."))
				]
				// "调试" 推演数据按钮
				+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f).AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Debug_Deduction_Deduction", "Debug Deduction Data"))
					.HAlign(HAlign_Center)
					.IsEnabled(this, &SSimpleNumericalDeductionWidget::IsDebugDeductionData)
					.OnClicked(this, &SSimpleNumericalDeductionWidget::DebugDeductionData)
					.ToolTipText(LOCTEXT("Debug_Deduction_DeductionTip", "Debug deduction deduction data according to the table."))
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
	// 渲染一个弹出提示, 表明你点了Save按钮并成功储存了SND的样式数据.
	OpenMessageDialog(LOCTEXT("SNDWidget_SaveAsDefault", "Storage SND SLATE Data Successfully."));
	return FReply::Handled();
}

bool SSimpleNumericalDeductionWidget::IsEnableToCSV() const
{
	if (const USNDObjectSettings* SND = GetDefault<USNDObjectSettings>()) {
		bool bHasDeduced = false;// 声明 是否执行了"推导行为"
		for (auto& MainTmp : SND->AttributeDatas) {
			for (auto& Tmp : MainTmp.AttributeDatas) {
				// 被推演的浮点个数大于2才能证明 已执行了"推导行为"
				if (Tmp.DeduceValue.Num() >= 2) {
					bHasDeduced = true;
					break;
				}
			}
		}
		// 路径不为空 && 推导次数不为0 && 推导行为已执行
		return !SND->CSVSavePath.Path.IsEmpty() && SND->DeductionNumber > 0 && bHasDeduced;
	}

	return false;
}

FReply SSimpleNumericalDeductionWidget::SaveAsCSV()
{
	if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
		// I: 扫描多属性表,针对单个属性表
		for (auto& MainTmp : SND->AttributeDatas) {
			// 先定义拼出1个 CSV路径
			FString CSVFilename = SND->CSVSavePath.Path / MainTmp.TableName.ToString() + TEXT(".csv");
			// 待加工的CSV数据, 它是一串字符
			TArray<FString> CSVData;

			// 1.1 构建头部, 类似于 
			//---,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18
			FString& HeadString = CSVData.AddDefaulted_GetRef();
			HeadString += TEXT("---,");
			for (int32 i = 0; i < SND->DeductionNumber; i++) {
				HeadString += FString::FromInt(i + 1) + TEXT(",");
			}
			HeadString.RemoveFromEnd(TEXT(","));

			// 1.2 构建内容, 诸如下面
			// II: 扫描单个属性表, 针对单条属性
			for (auto& TmpAttri : MainTmp.AttributeDatas) {
				FString& ContentString = CSVData.AddDefaulted_GetRef();
				//构建Key
				ContentString += TmpAttri.Key.ToString() + TEXT(",");
				//构建一系列尾数的属性值
				for (auto& DeducedVal : TmpAttri.DeduceValue) {
					ContentString += DeducedVal + TEXT(",");
				}
				ContentString.RemoveFromEnd(TEXT(","));
				// 最终产出类似于 MaxHealth,5,7,14,20,30,40,60,80,110,150,200,260,340,400,559,654,734,890
			}

			// III: 把处理好的CSV数据 存储至 CSV路径里.
			FFileHelper::SaveStringArrayToFile(CSVData, *CSVFilename);
		}
		// IV: 借助平台接口, 打开弹出 导出的CSV
		FPlatformProcess::ExploreFolder(*SND->CSVSavePath.Path);
	}

	// 渲染一个弹出提示, 表明你点了SaveAsCsv按钮并成功储存了SND的样式数据.
	OpenMessageDialog(LOCTEXT("SNDWidget_SaveAsCSV", "Storage As CSV Successfully."));
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
						// 归属的单属性记录至参数包
						NAEParam.Key = Tmp_attri.Key.ToString();
						// Tmp_attri.DeduceValue.Add(Tmp_attri.Value);// 推导值先默认给一份
						// 注意上面那句有隐含bug,由于CSV对逗号敏感,故处理技能Tag的时候携带逗号会诱发bug
						// 处理单属性的 推导值; 由于技能标签里有逗号, 故推荐用 连接符替换CSV敏感的逗号
						Tmp_attri.DeduceValue.Add(Tmp_attri.Value.Replace(TEXT(","), TEXT("-")));

						// 推演 单属性
						for (int32 i = 0; i < (SND->DeductionNumber - 1); ++i) {// 注意是SND->DeductionNumber - 1, 原因是有第一列留给了Lv, Hp, ComboAttack这些字段
							NAEParam.Value = FCString::Atof(*Tmp_attri.DeduceValue.Last());// 用推导浮点集的最新浮点填充参数包
							NAEParam.Count = i + 2;
							NAEParam.Coefficient = Tmp_attri.Coefficient;

							switch (Tmp_attri.AttributeDataType) {
								case EDeduceAttributeDataType::DEDUCETYPE_INT32:
								{
									float InValue = InObject->GetInt32AlgorithmValue(NAEParam);
									// 更新单属性的 "被推导出的浮点"
									Tmp_attri.DeduceValue.Add(FString::SanitizeFloat(InValue));
									break;
								}
								case EDeduceAttributeDataType::DEDUCETYPE_FLOAT:
								{
									int32 InValue = InObject->GetFloatAlgorithmValue(NAEParam);
									Tmp_attri.DeduceValue.Add(FString::FromInt(InValue));
									break;
								}
								case EDeduceAttributeDataType::DEDUCETYPE_STRING:
								{
									Tmp_attri.DeduceValue.Add(InObject->GetStringAlgorithmValue(NAEParam));
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	// 渲染一个弹出提示, 表示推演过程成功
	OpenMessageDialog(LOCTEXT("SNDWidget_GenerateDeduction", "Generate Deduce Progress Successfully."));
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

	// 通知成功生成的这个 属性表
	OpenMessageDialog(LOCTEXT("SNDWidget_GenerateAttributeTable", "Generate Attribute Table Successfully."));
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

/** 是否满足调试条件 */
bool SSimpleNumericalDeductionWidget::IsDebugDeductionData() const
{
	return this->IsGenerateDeduction();
}

/** 调试推演数据 */
FReply SSimpleNumericalDeductionWidget::DebugDeductionData()
{
	// 使用主module生成调试表
	FSimpleNumericalDeductionModule& SimpleNumericalDeductionModul = FModuleManager::LoadModuleChecked<FSimpleNumericalDeductionModule>("SimpleNumericalDeduction");
	SimpleNumericalDeductionModul.SpawnDebugDeductionTable();

	return FReply::Handled();
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

void SSimpleNumericalDeductionWidget::OpenMessageDialog(const FText& InText)
{
	FMessageDialog::Open(EAppMsgType::YesNo, InText);
}

#undef LOCTEXT_NAMESPACE// 终止定义本地化操作
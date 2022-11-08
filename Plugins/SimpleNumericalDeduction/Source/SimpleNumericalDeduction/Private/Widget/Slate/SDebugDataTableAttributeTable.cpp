#include "Widget/Slate/SDebugDataTableAttributeTable.h"
#include "PropertyEditorModule.h"
#include "Settings/SNDNumericalBalanceDebugSettings.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"

#define LOCTEXT_NAMESPACE "SDebugDataTableAttributeTable"

void SDebugDataTableAttributeTable::Construct(const FArguments& InArgs)
{
	// 先加载属性编辑器模块
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");
	// 仿照源码定义一些细节参数
	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowMultipleTopLevelObjects = true;
		DetailsViewArgs.bShowPropertyMatrixButton = false;
	}
	// 创建属性编辑器下的 1个细节面板
	TSharedPtr<class IDetailsView> ConfigPanel = PropertyModule.CreateDetailView(DetailsViewArgs);
	// 细节面板内塞入日志系统所需要的 BalanceDebugSettings
	TArray<UObject*> SourceObjects;
	{
		SourceObjects.Add(const_cast<USNDNumericalBalanceDebugSettings*>(GetDefault<USNDNumericalBalanceDebugSettings>()));
	}
	ConfigPanel->SetObjects(SourceObjects);

	/** 所有ChildSlot */
	ChildSlot
	[
		SNew(SVerticalBox)
		///
		+ SVerticalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).AutoHeight().Padding(4.f, 2.f, 4.f, 2.f)
		[
			SNew(SButton)
			.Text(LOCTEXT("SND_Generate", "Generate"))
			.HAlign(HAlign_Center)
			.OnClicked(this, &SDebugDataTableAttributeTable::Generate)
			.ToolTipText(LOCTEXT("SND_GenerateTip", "Store configuration locally."))
		]
		/// 反射界面 (来自BalanceDebugSetting这个数据结构)
		+ SVerticalBox::Slot().AutoHeight()
		[
			/** 具备数值平衡数据结构的Config Panel放在这个Slot */
			ConfigPanel.ToSharedRef()
		]

		/// 日志
		+ SVerticalBox::Slot().FillHeight(1.f).Padding(4.f, 2.f, 4.f, 2.f)
		[
			SNew(SScrollBox).Orientation(Orient_Vertical)
			
		]

		/// 底部的button
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SHorizontalBox)
			//
			+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f)
			[
				SNew(SButton)
				.Text(LOCTEXT("SND_SaveAsText", "Save as Text"))
				.HAlign(HAlign_Center)
				.OnClicked(this, &SDebugDataTableAttributeTable::SaveAsText)
				.ToolTipText(LOCTEXT("SND_SaveAsTextTip", "Store configuration locally."))
			]
			//
			+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Right).Padding(4.f, 2.f, 4.f, 2.f).AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("SND_ClearLog_csv", "Clear Log"))
				.HAlign(HAlign_Center)
				.OnClicked(this, &SDebugDataTableAttributeTable::ClearLog)
				.ToolTipText(LOCTEXT("SND_ClearLog_csvTip", "This function is mainly used for calling gameplay system after exporting deduction results."))
			]
		]
	];
}

FReply SDebugDataTableAttributeTable::SaveAsText()
{

	return FReply::Handled();
}

FReply SDebugDataTableAttributeTable::ClearLog()
{

	return FReply::Handled();
}

FReply SDebugDataTableAttributeTable::Generate()
{

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
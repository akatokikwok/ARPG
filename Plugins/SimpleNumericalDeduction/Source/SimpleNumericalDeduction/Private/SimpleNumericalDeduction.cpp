// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleNumericalDeduction.h"
#include "SimpleNumericalDeductionStyle.h"
#include "SimpleNumericalDeductionCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widget/SSimpleNumericalDeductionWidget.h"

static const FName SimpleNumericalDeductionTabName("SimpleNumericalDeduction");

#define LOCTEXT_NAMESPACE "FSimpleNumericalDeductionModule"

void FSimpleNumericalDeductionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleNumericalDeductionStyle::Initialize();
	FSimpleNumericalDeductionStyle::ReloadTextures();

	FSimpleNumericalDeductionCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSimpleNumericalDeductionCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSimpleNumericalDeductionModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSimpleNumericalDeductionModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SimpleNumericalDeductionTabName, FOnSpawnTab::CreateRaw(this, &FSimpleNumericalDeductionModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSimpleNumericalDeductionTabTitle", "SimpleNumericalDeduction"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	/** 这一步 初始化 曲线编辑器table的外观与内容布局 */
	AttributeCurveTable.InitLayout();
}

void FSimpleNumericalDeductionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSimpleNumericalDeductionStyle::Shutdown();

	FSimpleNumericalDeductionCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SimpleNumericalDeductionTabName);
}

/** 标签页内生成一个数值推演控件 */
TSharedRef<SDockTab> FSimpleNumericalDeductionModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
// 注释掉UE原生的内容
// 	FText WidgetText = FText::Format(
// 		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
// 		FText::FromString(TEXT("FSimpleNumericalDeductionModule::OnSpawnPluginTab")),
// 		FText::FromString(TEXT("SimpleNumericalDeduction.cpp"))
// 		);

	/** 在这个标签页内生成一个数值推演控件 */
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)// 改为水平填充
			.VAlign(VAlign_Fill)// 改为垂直填充
			[
				SNew(SSimpleNumericalDeductionWidget)
			]
		];
}

void FSimpleNumericalDeductionModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SimpleNumericalDeductionTabName);
}

/** 生成曲线编辑器table, 用多属性 */
void FSimpleNumericalDeductionModule::SpawnAttributeCurveTable(FDeduceAttributeDataTables& InDeduceAttributeDataTables)
{
	AttributeCurveTable.Construct(InDeduceAttributeDataTables);
}

/** 生成曲线编辑器table, 用单属性 */
void FSimpleNumericalDeductionModule::SpawnAttributeCurveTable(FDeduceAttributeData& InDeduceAttributeDataTabl)
{
	AttributeCurveTable.Construct(InDeduceAttributeDataTabl);
}

/** 生成调试表 */
void FSimpleNumericalDeductionModule::SpawnDebugDeductionTable()
{

}

void FSimpleNumericalDeductionModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSimpleNumericalDeductionCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSimpleNumericalDeductionCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleNumericalDeductionModule, SimpleNumericalDeduction)
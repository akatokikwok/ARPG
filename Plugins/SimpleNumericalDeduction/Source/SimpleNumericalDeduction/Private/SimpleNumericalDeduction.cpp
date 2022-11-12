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

	//////////////////////////////////////////////////////////////////////////
	// 模块启动时候做的一些任务

	/** 这一步 初始化 曲线编辑器table的外观与内容布局 */
	AttributeCurveTable.InitLayout();

	/** 初始化 调试推导数据接口的外观内容布局 */
	DebugAttributeDeduceTable.InitLayout();

	/** 自定义 对象 */
	this->RegisterObjectCustomizations();

	/** 自定义注册各变量属性 */
	this->RegisterPropertyTypeCustomizations();
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

	/** 模块结束后管理的一些卸载任务 */
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor")) {
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		// I. 扫描存放注册变量的容器TSet
		for (auto It = RegisteredPropertyTypes.CreateConstIterator(); It; ++It) {
			if (It->IsValid()) {
				PropertyEditorModule.UnregisterCustomPropertyTypeLayout(*It);// 卸载那些个注册变量
			}
		}
		// 通知一下自定义模块被改变
		PropertyEditorModule.NotifyCustomizationModuleChanged();
	}
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

/** 注册当前自定义的属性变量对象 */
void FSimpleNumericalDeductionModule::RegisterPropertyTypeCustomizations()
{
	// 注册单个变量, 需要指定名字和代理
	this->RegisterCustomPropertyTypeLayout("", FOnGetPropertyTypeCustomizationInstance::CreateStatic());
}

/** 注册单个变量用的方法, 需要指定名字和代理 */
void FSimpleNumericalDeductionModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);
	// 先把本变量存进容器,方便模块结束后管理销毁.
	RegisteredPropertyTypes.Add(PropertyTypeName);
	// 因为需要注册变量, 所以导入虚幻的模块PropertyEditor
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// 使用虚幻模块的API把某种数据结构注册起来
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
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
	DebugAttributeDeduceTable.Construct();
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
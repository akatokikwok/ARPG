// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleNumericalDeduction.h"
#include "SimpleNumericalDeductionStyle.h"
#include "SimpleNumericalDeductionCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

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

TSharedRef<SDockTab> FSimpleNumericalDeductionModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSimpleNumericalDeductionModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SimpleNumericalDeduction.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FSimpleNumericalDeductionModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SimpleNumericalDeductionTabName);
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
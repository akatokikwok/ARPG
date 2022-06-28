// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleCombatEditor.h"
#include "SimpleCombatEditorStyle.h"
#include "SimpleCombatEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName SimpleCombatEditorTabName("SimpleCombatEditor");

#define LOCTEXT_NAMESPACE "FSimpleCombatEditorModule"

void FSimpleCombatEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleCombatEditorStyle::Initialize();
	FSimpleCombatEditorStyle::ReloadTextures();

	FSimpleCombatEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSimpleCombatEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSimpleCombatEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSimpleCombatEditorModule::RegisterMenus));
}

void FSimpleCombatEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSimpleCombatEditorStyle::Shutdown();

	FSimpleCombatEditorCommands::Unregister();
}

void FSimpleCombatEditorModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FSimpleCombatEditorModule::PluginButtonClicked()")),
							FText::FromString(TEXT("SimpleCombatEditor.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FSimpleCombatEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSimpleCombatEditorCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSimpleCombatEditorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleCombatEditorModule, SimpleCombatEditor)
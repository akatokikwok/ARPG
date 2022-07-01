// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleCombatEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSimpleCombatEditorModule"

void FSimpleCombatEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SimpleCombatEditor", "Execute SimpleCombatEditor action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE

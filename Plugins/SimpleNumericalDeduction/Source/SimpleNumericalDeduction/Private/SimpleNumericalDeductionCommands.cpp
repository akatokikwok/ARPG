// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleNumericalDeductionCommands.h"

#define LOCTEXT_NAMESPACE "FSimpleNumericalDeductionModule"

void FSimpleNumericalDeductionCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SimpleNumericalDeduction", "Bring up SimpleNumericalDeduction window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE

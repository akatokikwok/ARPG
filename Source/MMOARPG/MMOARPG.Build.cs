// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMOARPG : ModuleRules
{
	public MMOARPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "SimpleThread",
            "SimpleNetChannel",
            "MMOARPGCommon",
            "Slate",
            "SlateCore",
            "SimpleActorBrowsing",
            "SimpleCombat",
            "SimpleAdvancedAnimation",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "SimpleDrawText",
            "AIModule",
            "SimpleHotUpdate",
            "SimpleNumericalDeductionRuntime",
            "SimplePopup",
            "SimpleRuntimeGamePrinting",
            "Niagara",
        });
	}
}

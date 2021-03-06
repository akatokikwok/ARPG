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
			"SimpleNetChannel",
			"SimpleHTTP",
			"SimpleThread",
			"MMOARPGCommon",
			"Slate",
			"SlateCore",
			"SimpleActorBrowsing",
			"SimpleCombat",
			"SimpleCombatEditor",
			"SimpleAdvancedAnimation",
            "GameplayAbilities",
            "GameplayTasks",
			"GameplayTags",
            "DataRegistry",
			"SimpleDrawText",
			"AIModule",
        });
	}
}

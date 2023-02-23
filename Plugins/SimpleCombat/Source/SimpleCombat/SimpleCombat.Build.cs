// Some copyright should be here...

using UnrealBuildTool;

public class SimpleCombat : ModuleRules
{
    public SimpleCombat(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "GameplayTags",
                "GameplayAbilities",
                "Niagara",
                "USDSchemas",
                "GameplayCameras",
                "Niagara",
				// ... add other public dependencies that you statically link with here ...
			}
            );

        // 仅在编辑器下(比如蒙太奇界面)要添加此部分模块逻辑
        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Persona",
            });
        }

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG",
				
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}

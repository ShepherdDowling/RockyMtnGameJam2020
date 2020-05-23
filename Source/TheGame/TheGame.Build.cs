// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheGame : ModuleRules
{
	public TheGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true; // Include What You Use (to ensure that no un-needed files are used)
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", "CoreUObject", "Engine", "InputCore", 
			"HeadMountedDisplay", "Slate", "SlateCore" 
		});
	}
}

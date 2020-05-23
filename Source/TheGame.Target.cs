// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheGameTarget : TargetRules
{
	public TheGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		bUseUnityBuild = false;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("TheGame");
	}
}

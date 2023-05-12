// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Gifters : ModuleRules
{
	public Gifters(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "GameplayTasks", "AIModule", "NavigationSystem" });
	}
}

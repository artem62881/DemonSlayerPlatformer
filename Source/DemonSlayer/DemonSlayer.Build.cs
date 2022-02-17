// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DemonSlayer : ModuleRules
{
	public DemonSlayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "NavigationSystem" });
		
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateIncludePaths.AddRange(new string[] { Name });
	}
}

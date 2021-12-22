// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KatanaProject : ModuleRules
{
	public KatanaProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		PublicDependencyModuleNames.AddRange(new string[] 
		{ "Core", "CoreUObject", "Engine", "InputCore", 
			"ApexDestruction", "AIModule", "GameplayTasks", "NavigationSystem", "UMG",
			"MovieScene", "LevelSequence", "MediaAssets", "Networking", "Sockets", "Niagara"
		});
	}
}

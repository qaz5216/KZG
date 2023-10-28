// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KZG : ModuleRules
{
	public KZG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" ,"Niagara", "AIModule", "NavigationSystem", "UMG", "SlateCore", "OnlineSubsystem", "OnlineSubsystemSteam"});
	}
}

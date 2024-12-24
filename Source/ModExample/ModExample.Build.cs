// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModExample : ModuleRules
{
	public ModExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "GameFeatures", "Projects" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"});
	}
}

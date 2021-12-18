// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkateTraffic : ModuleRules
{
	public SkateTraffic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "UMG" });

		PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
	}
}

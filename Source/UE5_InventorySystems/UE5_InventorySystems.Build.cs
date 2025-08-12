// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5_InventorySystems : ModuleRules
{
	public UE5_InventorySystems(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UE5_InventorySystems",
			"UE5_InventorySystems/Variant_Platforming",
			"UE5_InventorySystems/Variant_Combat",
			"UE5_InventorySystems/Variant_Combat/AI",
			"UE5_InventorySystems/Variant_SideScrolling",
			"UE5_InventorySystems/Variant_SideScrolling/Gameplay",
			"UE5_InventorySystems/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

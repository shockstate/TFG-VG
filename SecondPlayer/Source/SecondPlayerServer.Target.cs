// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class SecondPlayerServerTarget : TargetRules
{
	public SecondPlayerServerTarget(TargetInfo Target)
	{
		Type = TargetType.Server;
        bUsesSteam = true;
    }

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
        OutExtraModuleNames.Add("SecondPlayer");
	}
}

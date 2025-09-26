// Fill out your copyright notice in the Description page of Project Settings.

#include "DTFrameTemplete.h"
#include "Misc/ConfigCacheIni.h"

#define LOCTEXT_NAMESPACE "FDTFrameTempleteModule"

void FDTFrameTempleteModule::StartupModule()
{
	TArray<FString> Result;
	FString Filename= FPaths::ProjectPluginsDir()+TEXT("DTFrameTemplete/doc/DTFrameTempleteInputSetting.ini");
	if (GConfig->GetSection(TEXT("/Script/DTFrameTemplete.DTFrameTempleteInputSetting"), Result,Filename))
	{
		FString DefaultInputFilename = FPaths::ProjectConfigDir() + TEXT("DefaultInput.ini");
		TArray<FString>ActionMappings;
		TArray<FString>AxisMappings;
		for (auto Item = Result.CreateConstIterator(); Item; ++Item)
		{
			FString Key,Value;
			(*Item).Split(TEXT("="), &Key, &Value);
			if (Key.Equals(TEXT("+ActionMappings")))
			{
				ActionMappings.AddUnique(Value);
			}
			else if (Key.Equals(TEXT("+AxisMappings")))
			{
				AxisMappings.AddUnique(Value);
			}
		}
		if (GConfig->GetSection(TEXT("/Script/Engine.InputSettings"), Result, DefaultInputFilename))
		{
			for (auto Item = Result.CreateConstIterator(); Item; ++Item)
			{
				FString Key, Value;
				(*Item).Split(TEXT("="), &Key, &Value);
				if (Key.Equals(TEXT("+ActionMappings")))
				{
					ActionMappings.AddUnique(Value);
				}
				else if (Key.Equals(TEXT("+AxisMappings")))
				{
					AxisMappings.AddUnique(Value);
				}
			}
		}
		GConfig->SetArray(TEXT("/Script/Engine.InputSettings"),TEXT("+ActionMappings"), ActionMappings, DefaultInputFilename);
		GConfig->SetArray(TEXT("/Script/Engine.InputSettings"), TEXT("+AxisMappings"), AxisMappings, DefaultInputFilename);
	}

}

void FDTFrameTempleteModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDTFrameTempleteModule, DTFrameTemplete)

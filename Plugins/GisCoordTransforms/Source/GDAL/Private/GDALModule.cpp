// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GDALModule.h"

#include "Core.h"
#include "Interfaces/IPluginManager.h"

#include "gdal_priv.h"
#include "gdalwarper.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"

static void* LibraryHandle;

#define LOCTEXT_NAMESPACE "FGDALModule"

void FGDALModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module


	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("GisCoordTransforms")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	FString DllPath = FPaths::Combine(*BaseDir, TEXT("Source/GDAL/dll/Win64"));
	FPlatformProcess::AddDllDirectory(*DllPath);
	LibraryPath = FPaths::Combine(*DllPath, TEXT("gdal300.dll"));
#endif // PLATFORM_WINDOWS
	LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (!LibraryHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load third party library: gdal300.dll "));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("============= GDAL  OnStartup  ============="));

	UE_LOG(LogTemp, Log, TEXT("==== GDALAllRegister ===="));

	GDALAllRegister();
	GDALDriverManager* pDriverManager = GetGDALDriverManager();
	for (size_t i = 0; i < pDriverManager->GetDriverCount(); i++)
	{
		const char* pDriverDesc = pDriverManager->GetDriver(i)->GetDescription();
		UE_LOG(LogTemp, Log, TEXT("GDALDriver[%d]:%s"), i, ANSI_TO_TCHAR(pDriverDesc));
	}

	CPLSetConfigOption("GDAL_CACHEMAX", "1000");
	CPLSetConfigOption("OGR_PG_CURSOR_PAGE", "10000");
	CPLSetConfigOption("SHAPE_ENCODING", "");

	UE_LOG(LogTemp, Log, TEXT("==== GDALAllRegister ===="));
}

void FGDALModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(LogTemp, Log, TEXT("============= GDAL OnShutdown ============="));

	FPlatformProcess::FreeDllHandle(LibraryHandle);
	LibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGDALModule, GDAL)
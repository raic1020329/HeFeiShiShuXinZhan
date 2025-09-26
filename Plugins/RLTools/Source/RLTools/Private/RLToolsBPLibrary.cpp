// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLToolsBPLibrary.h"
//#include "IDesktopPlatform.h"
//#include "DesktopPlatformModule.h"
#include "Engine/StaticMesh.h"
#include"Runtime/Core/Public/Misc/Paths.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/WindowsHWrapper.h"
#include <windows.h>
#include <string>
#include "Windows/HideWindowsPlatformTypes.h"
#include <ShlDisp.h>
#include <shellapi.h>
#include "Components/LineBatchComponent.h"


//FProcHandle URLToolsBPLibrary::pHandle = NULL;

#if PLATFORM_WINDOWS
#pragma optimize("",off)
#endif

URLToolsBPLibrary::URLToolsBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool URLToolsBPLibrary::OpenWindowsMovie(TArray<FString>& FilePath)
{
	FString ExtensionStr = ".";
	bool bOpen = false;
	/*IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
		bOpen = DesktopPlatform->OpenFileDialog(nullptr, TEXT("选取文件"),
			FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()), TEXT(""),
			*ExtensionStr, EFileDialogFlags::None, FilePath);
	else
		return bOpen;*/
	return bOpen;
}

void URLToolsBPLibrary::GetBoundVertexVextur2D(const FVector& Origin, const FVector& BoxExtent, FVector2D& LeftUp,
	FVector2D& LeftDown, FVector2D& RightUp, FVector2D& RightDown)
{
	LeftUp = FVector2D(Origin.X - BoxExtent.X, Origin.Y - BoxExtent.Y);
	LeftDown = FVector2D(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y);
	RightUp = FVector2D(Origin.X + BoxExtent.X, Origin.Y - BoxExtent.Y);
	RightDown = FVector2D(Origin.X + BoxExtent.X, Origin.Y + BoxExtent.Y);
}

#if WITH_EDITOR
void URLToolsBPLibrary::SetDfRes(UStaticMesh* SM, float TargetDFRes)
{
	if (SM == nullptr)
		return;

	if (SM && SM->GetSourceModels().Num() > 0)
	{
		SM->Modify();
		for (int32 i = 0; i < SM->GetSourceModels().Num(); i++)
		{
			FStaticMeshSourceModel& Model = SM->GetSourceModel(i);
			Model.BuildSettings.DistanceFieldResolutionScale = TargetDFRes;
		}
		SM->Build();
	}
}
#endif

static FProcHandle pHandle;
void URLToolsBPLibrary::OpenExternalApp(const FString& AppPath)
{

	pHandle = FPlatformProcess::CreateProc(*AppPath, nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
	UE_LOG(LogTemp, Warning, TEXT("%hs"), __FUNCTION__);
}

void URLToolsBPLibrary::CloseExternalApp()
{
	if (pHandle.IsValid())
	{
		FPlatformProcess::TerminateProc(pHandle);
		pHandle.Reset();
		UE_LOG(LogTemp, Warning, TEXT("%hs"), __FUNCTION__);
	}
	else
	{
		//输出日志
		UE_LOG(LogTemp, Warning, TEXT("Close None : %hs"), __FUNCTION__);
	}
}

void URLToolsBPLibrary::ExecuteExternalApp(const FString& AppPath, const FString& Parameters, int32 nShowCmd)
{
	std::string str_path = TCHAR_TO_UTF8(*AppPath);
	std::wstring wstr_path;
	wstr_path.assign(str_path.begin(), str_path.end());
	if (!Parameters.IsEmpty())
	{
		std::string str_parameter = TCHAR_TO_UTF8(*Parameters);
		std::wstring wstr_parameter;
		wstr_parameter.assign(str_parameter.begin(), str_parameter.end());

		ShellExecute(NULL, L"open", wstr_path.c_str(), wstr_parameter.c_str(), NULL, nShowCmd);
		//auto a = ShellExecute(NULL, L"open", wstr_path.c_str(), wstr_parameter.c_str(), NULL, nShowCmd);
		//UE_LOG(LogTemp, Warning, TEXT("%d"), a);
		//SW_SHOWMAXIMIZED
	}
	else
	{
		ShellExecute(NULL, L"open", wstr_path.c_str(), NULL, NULL, nShowCmd);
	}
}

void URLToolsBPLibrary::KillProcess(const FString& ProcessName)
{
	std::string process = std::string("TASKKILL /F /IM ") + TCHAR_TO_UTF8(*ProcessName);
	system(process.c_str());
	UE_LOG(LogTemp, Warning, TEXT("%hs"), __FUNCTION__);
}

void URLToolsBPLibrary::ShowVirtualKeyboard()
{
	ShellExecute(NULL, L"open", L"osk.exe", NULL, NULL, SW_SHOWNORMAL);
}

ULineBatchComponent* URLToolsBPLibrary::GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines,
	float LifeTime, bool bDepthIsForeground)
{
	return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : nullptr);
}

float URLToolsBPLibrary::GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent)
{
	return bPersistent ? -1.0f : ((LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime);
}

void URLToolsBPLibrary::DrawDebugLineWithShipping(const UObject* WorldContextObject, const FVector& Start,
	const FVector& End, const FLinearColor& Color, uint8 DepthPriority, const float Thickness, const float LifeTime)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (GEngine->GetNetMode(World) != NM_DedicatedServer)
		{
			// this means foreground lines can't be persistent 
			if (ULineBatchComponent* const LineBatcher = GetDebugLineBatcher(World, false, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				float const LineLifeTime = GetDebugLineLifeTime(LineBatcher, LifeTime, false);
				LineBatcher->DrawLine(Start, End, Color, DepthPriority, Thickness, LineLifeTime);
			}
		}
	}
}

void URLToolsBPLibrary::DrawDebugSphereWithShipping(const UObject* WorldContextObject, FVector const& Center,
	float Radius, int32 Segments, const FLinearColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority,
	float Thickness)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		// no debug line drawing on dedicated server
		if (GEngine->GetNetMode(World) != NM_DedicatedServer)
		{
			// this means foreground lines can't be persistent 
			if (ULineBatchComponent* const LineBatcher = GetDebugLineBatcher(World, bPersistentLines, LifeTime, (DepthPriority == SDPG_Foreground)))
			{
				float LineLifeTime = GetDebugLineLifeTime(LineBatcher, LifeTime, bPersistentLines);

				// Need at least 4 segments
				Segments = FMath::Max(Segments, 4);

				FVector Vertex1, Vertex2, Vertex3, Vertex4;
				const float AngleInc = 2.f * PI / float(Segments);
				int32 NumSegmentsY = Segments;
				float Latitude = AngleInc;
				int32 NumSegmentsX;
				float Longitude;
				float SinY1 = 0.0f, CosY1 = 1.0f, SinY2, CosY2;
				float SinX, CosX;

				TArray<FBatchedLine> Lines;
				Lines.Empty(NumSegmentsY * Segments * 2);
				while (NumSegmentsY--)
				{
					SinY2 = FMath::Sin(Latitude);
					CosY2 = FMath::Cos(Latitude);

					Vertex1 = FVector(SinY1, 0.0f, CosY1) * Radius + Center;
					Vertex3 = FVector(SinY2, 0.0f, CosY2) * Radius + Center;
					Longitude = AngleInc;

					NumSegmentsX = Segments;
					while (NumSegmentsX--)
					{
						SinX = FMath::Sin(Longitude);
						CosX = FMath::Cos(Longitude);

						Vertex2 = FVector((CosX * SinY1), (SinX * SinY1), CosY1) * Radius + Center;
						Vertex4 = FVector((CosX * SinY2), (SinX * SinY2), CosY2) * Radius + Center;

						Lines.Add(FBatchedLine(Vertex1, Vertex2, Color, LineLifeTime, Thickness, DepthPriority));
						Lines.Add(FBatchedLine(Vertex1, Vertex3, Color, LineLifeTime, Thickness, DepthPriority));

						Vertex1 = Vertex2;
						Vertex3 = Vertex4;
						Longitude += AngleInc;
					}
					SinY1 = SinY2;
					CosY1 = CosY2;
					Latitude += AngleInc;
				}
				LineBatcher->DrawLines(Lines);
			}
		}
	}
}

int64 URLToolsBPLibrary::GetTimeSecondstamp()
{
	FDateTime Time = FDateTime::Now();

	int64 Timestamp = Time.ToUnixTimestamp();

	return Timestamp;
}

int64 URLToolsBPLibrary::GetTimeMillisecondstamp()
{
	int64 timestamp = (FDateTime::Now() - FDateTime(1970, 1, 1)).GetTotalMilliseconds();
	return timestamp;
}

FDateTime URLToolsBPLibrary::GetTimeFromstamp(int64 UnixTime)
{
	FDateTime Time = FDateTime::FromUnixTimestamp(UnixTime);
	return Time;
}

bool URLToolsBPLibrary::bRunPIE(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->WorldType == EWorldType::PIE;
}

FString URLToolsBPLibrary::GetMachineID()
{
	return FGenericPlatformMisc::GetMacAddressString();
}


void URLToolsBPLibrary::SetWindowTopMost(uint8 bNeedTopMost, int32 x, int32 y, int32 cx, int32 xy)
{
	if (GEngine && GEngine->GameViewport)
	{
		void* WindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		HWND Hwnd = static_cast<HWND>(WindowHandle);
		SetWindowPos(Hwnd, bNeedTopMost ? HWND_TOPMOST : HWND_NOTOPMOST, x, y, cx, xy, SWP_NOMOVE | SWP_NOSIZE);
	}
}



#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
#include <Components/LineBatchComponent.h>



UMyBlueprintFunctionLibrary::UMyBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UObject* UMyBlueprintFunctionLibrary::GetObjectByPath(UClass* UClassType, const FString& Name)
{
	if (Name.IsEmpty())
		return nullptr;

	return StaticLoadObject(UClassType ? UClassType : UObject::StaticClass(), NULL, *Name);
}

FString UMyBlueprintFunctionLibrary::FormatStringFromFloat(float Value, int32 DecimalLength)
{
	FString Format = TEXT("%.") + FString::FromInt(DecimalLength) + TEXT("f");
	static TCHAR PrintFormat[4096];
	int32 Count = Format.Len();
	CopyAssignItems(PrintFormat, *Format, Count);
	PrintFormat[Count] = 0;
	FString Result = FString::Printf(PrintFormat, Value);
	return Result;
}


#if PLATFORM_WINDOWS

#include "Windows/AllowWindowsPlatformTypes.h"
#include <time.h>
#include <mmsystem.h>
#include <rpcsal.h>
#include <gameux.h>
#include <ShlObj.h>
#include <IntShCut.h>
#include <shellapi.h>
#include <IPHlpApi.h>
#include <VersionHelpers.h>
#include "Windows/HideWindowsPlatformTypes.h"


FString UMyBlueprintFunctionLibrary::GetLocalIP()
{
	FString LocalIPString;
	IP_ADAPTER_INFO IpAddresses[16];
	ULONG OutBufferLength = sizeof(IP_ADAPTER_INFO) * 16;
	// Read the adapters
	uint32 RetVal = GetAdaptersInfo(IpAddresses, &OutBufferLength);
	if (RetVal == NO_ERROR)
	{
		PIP_ADAPTER_INFO AdapterList = IpAddresses;
		// Walk the set of addresses copying each one
		while (AdapterList)
		{
			// If there is an address to read
			if (AdapterList->AddressLength > 0)
			{
				// Copy the data and say we did
				LocalIPString = ANSI_TO_TCHAR((char*)(AdapterList->IpAddressList.IpAddress.String));
				if (!LocalIPString.Equals(TEXT("0.0.0.0")))
					break;
			}
			AdapterList = AdapterList->Next;
		}
	}
	return LocalIPString;
}
#else
FString UMyBlueprintFunctionLibrary::GetLocalIP()
{
	return TEXT("127.0.0.1");
}
#endif




bool UMyBlueprintFunctionLibrary::IsChineseCharacter(FString InStr)
{
	char* tmpChar = TCHAR_TO_ANSI(*InStr);

	int tmpLen = strlen(tmpChar);

	for (int i = 0; i < tmpLen; i++)
	{
		char h = tmpChar[i];
		if ((h < 0XA0B0 && h>122) || h > 0xfef7 || (h > 90 && h < 97) || (h > 57 && h < 65) || h < 48)
			return true;
	}
	return false;
}







TArray<int32> UMyBlueprintFunctionLibrary::DistanceSort(TMap<FVector, int32>& Map, UObject* WorldContnex)
{
	TArray<int32> res;
	if (Map.Num() > 0)
	{
		Map.KeySort(
			[&](const FVector& A, const FVector& B)->bool
			{
				float DistanceA = FVector::Dist2D(A, UGameplayStatics::GetPlayerCameraManager(WorldContnex, 0)->GetCameraLocation());
				float DistanceB = FVector::Dist2D(B, UGameplayStatics::GetPlayerCameraManager(WorldContnex, 0)->GetCameraLocation());
				if (DistanceA <= DistanceB)
				{
					return true;
				}
				return false;
			}
		);
		Map.GenerateValueArray(res);
	}

	return res;
}







TArray<float> UMyBlueprintFunctionLibrary::BubbleSortForFloatArray(TArray<float> FloatArray)
{
	for (int i = 0; i < FloatArray.Num(); i++) {
		float temp = 0.f;
		bool exchange = false;
		for (int j = FloatArray.Num() - 1; j > 0; j--) {
			if (FloatArray[j] > FloatArray[j - 1]) {
				temp = FloatArray[j];
				FloatArray[j] = FloatArray[j - 1];
				FloatArray[j - 1] = temp;
				exchange = true;
			}
		}
		if (!exchange) {
			break;
		}
	}

	return FloatArray;
}

TMap<FString, float> UMyBlueprintFunctionLibrary::SortData(TMap<FString, float> Data, bool Ascending)
{
	int32 length = Data.Num();

	TArray<FString> keys;
	TArray<float> values;

	Data.GetKeys(keys);
	Data.GenerateValueArray(values);



	TMap<FString, float> AfterSort;

	for (int32 i = 0; i < length; i++)
	{
		for (int32 j = 0; j < length - i - 1; j++)
		{
			if (Ascending)
			{
				if (values[j] > values[j + 1])
				{
					float valuetemp = values[j];
					values[j] = values[j + 1];
					values[j + 1] = valuetemp;

					FString keytemp = keys[j];
					keys[j] = keys[j + 1];
					keys[j + 1] = keytemp;

				}
			}
			else
			{
				if (values[j] < values[j + 1])
				{
					float valuetemp = values[j];
					values[j] = values[j + 1];
					values[j + 1] = valuetemp;

					FString keytemp = keys[j];
					keys[j] = keys[j + 1];
					keys[j + 1] = keytemp;

				}
			}

		}
	}

	AfterSort.Empty();
	for (int32 i = 0; i < length; i++)
	{
		AfterSort.Add(keys[i], values[i]);
	}

	return AfterSort;
}

TMap<FString, int32> UMyBlueprintFunctionLibrary::SortDataInt32(TMap<FString, int32> Data, bool Ascending)
{
	int32 length = Data.Num();

	TArray<FString> keys;
	TArray<int32> values;

	Data.GetKeys(keys);
	Data.GenerateValueArray(values);



	TMap<FString, int32> AfterSort;

	for (int32 i = 0; i < length; i++)
	{
		for (int32 j = 0; j < length - i - 1; j++)
		{
			if (Ascending)
			{
				if (values[j] > values[j + 1])
				{
					int32 valuetemp = values[j];
					values[j] = values[j + 1];
					values[j + 1] = valuetemp;

					FString keytemp = keys[j];
					keys[j] = keys[j + 1];
					keys[j + 1] = keytemp;

				}
			}
			else
			{
				if (values[j] < values[j + 1])
				{
					int32 valuetemp = values[j];
					values[j] = values[j + 1];
					values[j + 1] = valuetemp;

					FString keytemp = keys[j];
					keys[j] = keys[j + 1];
					keys[j + 1] = keytemp;

				}
			}

		}
	}

	AfterSort.Empty();
	for (int32 i = 0; i < length; i++)
	{
		AfterSort.Add(keys[i], values[i]);
	}

	return AfterSort;
}

int32 UMyBlueprintFunctionLibrary::ZellerCongruence(FDateTime Time)
{

	int32 Year = Time.GetYear();
	int32 Month = Time.GetMonth();
	int32 Day = Time.GetDay();

	if ((Month < 1) || (Month > 12) || ((Day < 1) || (Day > 31)))
	{
		return 0;
	}

	if (Month < 3)
	{
		Month += 12;
		Year -= 1;
	}

	int32 Century = Year / 100;
	int32 Y = Year % 100;

	int32 WeekDay;


	WeekDay = (Y + (Y / 4) + (Century / 4) + (5 * Century) + (26 * (Month + 1) / 10) + Day - 1) % 7;

	return WeekDay;


}


ULineBatchComponent* UMyBlueprintFunctionLibrary::GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground)
{
	return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : nullptr);
}

float UMyBlueprintFunctionLibrary::GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent)
{
	return bPersistent ? -1.0f : ((LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime);
}

void UMyBlueprintFunctionLibrary::DrawDebugLineWithShipping(const UObject* WorldContextObject,const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriority, const float Thickness, const float LifeTime)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
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

void UMyBlueprintFunctionLibrary::DrawDebugSphereWithShipping(const UObject* WorldContextObject, FVector const& Center, float Radius, int32 Segments, const FLinearColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
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

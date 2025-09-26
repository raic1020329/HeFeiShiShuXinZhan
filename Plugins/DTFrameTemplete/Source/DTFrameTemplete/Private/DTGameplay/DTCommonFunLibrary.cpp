// Fill out your copyright notice in the Description page of Project Settings.


#include "DTGameplay/DTCommonFunLibrary.h"
#include "DTGameplay/CameraAnchorPoint.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "DTGameplay/DTPawn.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include <Internationalization/Regex.h>
#include "Components/LineBatchComponent.h"
#include <Misc/DateTime.h>
#include <Engine/Texture2D.h>
//#include <ImageWriteQueue/Public/ImageWriteTypes.h>
#include <IImageWrapperModule.h>
#include <IImageWrapper.h>


void UDTCommonFunLibrary::ChangeCameraByTag(UObject* WorldContextObject, FName InCameraTag, int32 PlayerIndex)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ACameraAnchorPoint> It(World); It; ++It)
		{
			ACameraAnchorPoint* CameraAnchorPoint = *It;
			if (CameraAnchorPoint && CameraAnchorPoint->IsValidLowLevel())
			{
				if (CameraAnchorPoint->CameraTag == InCameraTag)
				{
					APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, PlayerIndex);
					if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
					{
						DTPawn->ChangeCamera(InCameraTag, CameraAnchorPoint->CameraSetting, CameraAnchorPoint->CameraLimit);
						break;
					}
				}
			}
		}
	}

}
void UDTCommonFunLibrary::ChangeCameraWithDelegate(UObject* WorldContextObject, FName InCameraTag, FCameraChangeComplete CompleteCallBack, int32 PlayerIndex)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ACameraAnchorPoint> It(World); It; ++It)
		{
			ACameraAnchorPoint* CameraAnchorPoint = *It;
			if (CameraAnchorPoint && CameraAnchorPoint->IsValidLowLevel())
			{
				if (CameraAnchorPoint->CameraTag == InCameraTag)
				{
					APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, PlayerIndex);
					if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
					{
						DTPawn->ChangeCamera(InCameraTag, CameraAnchorPoint->CameraSetting, CameraAnchorPoint->CameraLimit, CompleteCallBack);
						break;
					}
				}
			}
		}
	}
}

void UDTCommonFunLibrary::CameraFocusOnObject(UObject* WorldContextObject, FVector InLoc, float AnimTime/*=1.0f */, float Distance/*=100*/, bool bIsUseInstant/*=false*/, int32 PlayerIndex)
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, PlayerIndex);
	if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
	{
		FRotator Rot = DTPawn->GetActorRotation();
		DTPawn->CameraFocus(InLoc, Rot, AnimTime, Distance, bIsUseInstant);
	}
}

void UDTCommonFunLibrary::CameraFocusOnObjectFromRot(
	UObject* WorldContextObject,
	FVector InLoc,
	FRotator Rot,
	float AnimTime /*= 1.0f*/,
	float Distance /*= 100*/,
	bool bIsUseInstant /*= false*/, 
	int32 PlayerIndex)
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, PlayerIndex);
	if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
	{
		DTPawn->CameraFocus(InLoc, Rot, AnimTime, Distance, bIsUseInstant);
	}
}

void UDTCommonFunLibrary::CameraLookAtRotation(UObject* WorldContextObject, FVector InEyeLoc, FVector InTargetLoc, float AnimTime /*= 1.0f*/, bool bIsUseInstant /*= false*/)
{
	FRotator Rot = FRotationMatrix::MakeFromX(InTargetLoc - InEyeLoc).Rotator();
	float dis = FVector::Distance(InEyeLoc, InTargetLoc);
	CameraFocusOnObjectFromRot(WorldContextObject, InTargetLoc, Rot, AnimTime, dis / 100.f, bIsUseInstant);
}

void UDTCommonFunLibrary::CameraLookAtRotationArchBridge(UObject* WorldContextObject, FVector InEyeLoc, FVector InTargetLoc, float AnimTime /*= 1.0f*/, float degree/* = 80.f*/, float DisScaleVale/* = 1.2f*/, int32 PlayerIndex)
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, PlayerIndex);
	if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
	{
		FRotator Rot = FRotationMatrix::MakeFromX(InTargetLoc - InEyeLoc).Rotator();
		if (!InTargetLoc.Equals(DTPawn->GetActorLocation(), 10.f) || !Rot.Equals(DTPawn->GetActorRotation()))
		{
			DTPawn->CameraFocusArchBridge(InEyeLoc, InTargetLoc, AnimTime, degree, DisScaleVale);
		}
		else
		{
			CameraLookAtRotation(WorldContextObject, InEyeLoc, InTargetLoc, AnimTime);
		}
	}
}

void UDTCommonFunLibrary::SaveString(const FString& InPath, const FString& InDataArr)
{
	FFileHelper::SaveStringToFile(InDataArr, *InPath);
}

void UDTCommonFunLibrary::LoadString(const FString& InPath, FString& OutData)
{
	FFileHelper::LoadFileToString(OutData, *InPath);
}

void UDTCommonFunLibrary::SaveStringArray(const FString& InPath, const TArray<FString>& InDataArr)
{
	FFileHelper::SaveStringArrayToFile(InDataArr, *InPath);
}

void UDTCommonFunLibrary::LoadStringArray(const FString& InPath, TArray<FString>& OutData)
{
	FFileHelper::LoadFileToStringArray(OutData, *InPath);
}

bool UDTCommonFunLibrary::AutoParserJson(const FString& InContent, const FString& InStructParamaName, UObject* InObject)
{
	FCriticalSection m_Mutex;
	FScopeLock ScopeLock(&m_Mutex);//作用域锁
	for (TFieldIterator<FProperty> tPropertyIter(InObject->GetClass()); tPropertyIter; ++tPropertyIter)
	{
		if (tPropertyIter->GetName().Contains(InStructParamaName))
		{
			FProperty* spProperty = *tPropertyIter;
			UStruct* realDataStruct = CastField<FStructProperty>(spProperty)->Struct;
			if (!realDataStruct)
				return false;
			void* spDataPtr = tPropertyIter->ContainerPtrToValuePtr<void>(InObject);
			TSharedRef<TJsonReader<>> JsonRead = TJsonReaderFactory<>::Create(InContent);
			TSharedPtr<FJsonObject> JsonObjectRoot;
			if (FJsonSerializer::Deserialize(JsonRead, JsonObjectRoot))
			{
				return ParseJsonData(realDataStruct, spDataPtr, JsonObjectRoot, spProperty);
			}
			break;
		}
	}
	return false;
}

bool UDTCommonFunLibrary::ParseJsonData(UStruct* inStruct, void* inDataAddress, TSharedPtr<FJsonObject> inJsonData, FProperty* spProperty)
{
	if (nullptr == spProperty || !spProperty->IsValidLowLevel())
		return false;
	for (TMap<FString, TSharedPtr<FJsonValue>>::TIterator tJsonIter = inJsonData->Values.CreateIterator(); tJsonIter; ++tJsonIter)
	{
		for (TFieldIterator<FProperty> tPropertyIter(inStruct); tPropertyIter; ++tPropertyIter)
		{
			FProperty* tpProperty = *tPropertyIter;
			if (nullptr == tpProperty || !tpProperty->IsValidLowLevel())
				continue;
			//static const FString rd(TEXT("rd_tl"));
			FString key = tJsonIter->Key;

			const FString ProName = inStruct->GetAuthoredNameForField(tpProperty);
			if (ProName != key)
				continue;
			UpdatePropertyByJson(*tPropertyIter, inDataAddress, tJsonIter->Value, spProperty);
			break;
		}
	}
	return true;
}

void UDTCommonFunLibrary::UpdatePropertyByJson(FProperty* inProperty, void* inDataAddress, const TSharedPtr<FJsonValue>& inJsonValue, FProperty* spProperty)
{
	if (nullptr == spProperty || !spProperty->IsValidLowLevel())
		return;
	if (nullptr == inProperty || !inProperty->IsValidLowLevel())
		return;
	switch (inJsonValue->Type)
	{
	case EJson::Number:
	{
		if (!inProperty->IsA(FNumericProperty::StaticClass()))
			break;
		CastField<FNumericProperty>(inProperty)->SetNumericPropertyValueFromString(inProperty->ContainerPtrToValuePtr<void>(inDataAddress), *inJsonValue->AsString());
	}
	break;
	case EJson::Boolean:
	{
		if (!inProperty->IsA(FBoolProperty::StaticClass()))
			break;
		CastField<FBoolProperty>(inProperty)->SetPropertyValue_InContainer(inDataAddress, inJsonValue->AsBool());
	}
	break;
	case EJson::String:
	{
		if (!inProperty->IsA(FStrProperty::StaticClass()))
			break;
		CastField<FStrProperty>(inProperty)->SetPropertyValue_InContainer(inDataAddress, inJsonValue->AsString());
	}
	break;
	case EJson::Object:
	{
		if (!inProperty->IsA(FStructProperty::StaticClass()))
			break;
		void* tpChildDataAddress = inProperty->ContainerPtrToValuePtr<void>(inDataAddress);
		UStruct* tpChildStruct = CastField<FStructProperty>(inProperty)->Struct;
		ParseJsonData(tpChildStruct, tpChildDataAddress, inJsonValue->AsObject(), spProperty);
	}
	break;
	case EJson::Array:
	{
		if (!inProperty->IsA(FArrayProperty::StaticClass()))
			break;
		FArrayProperty* tpArrayProperty = CastField<FArrayProperty>(inProperty);
		FProperty* tpElementProperty = CastField<FArrayProperty>(inProperty)->Inner;
		void* tpArrayAddress = inProperty->ContainerPtrToValuePtr<void>(inDataAddress);
		FScriptArrayHelper_InContainer tArrayHelper(tpArrayProperty, inDataAddress);
		const TArray<TSharedPtr<FJsonValue>>& tArrayJsonValue = inJsonValue->AsArray();
		int32 tArraySize = tArrayJsonValue.Num();
		{
			tArrayHelper.Resize(tArraySize);
		}
		for (int32 tIndex = 0; tIndex < tArraySize; ++tIndex)
		{
			UpdatePropertyByJson(tpElementProperty, tArrayHelper.GetRawPtr(tIndex), tArrayJsonValue[tIndex], spProperty);
		}
	}
	break;
	default:
		break;
	}
}

FCameraSetting UDTCommonFunLibrary::GetCurrentCameraSetting(UObject* WorldContextObject) 
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
	if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
	{
		return DTPawn->GetCurrentCameAttribute();
	}
	return FCameraSetting();
}

FCameraLimit UDTCommonFunLibrary::GetCurrentCameraLimit(UObject* WorldContextObject)
{
	FCameraLimit Limit;
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
	if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
	{
		return DTPawn->CameraLimit;
	}
	return Limit;
}

void UDTCommonFunLibrary::SetCurrentCameraSetting(UObject* WorldContextObject, FCameraSetting Setting, FCameraLimit Limit)
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
	if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			for (TActorIterator<ACameraAnchorPoint> It(World); It; ++It)
			{
				ACameraAnchorPoint* CameraAnchorPoint = *It;
				if (CameraAnchorPoint && CameraAnchorPoint->IsValidLowLevel())
				{
					if (CameraAnchorPoint->CameraTag == DTPawn->CameraTag)
					{
						CameraAnchorPoint->InitCameraSetting(Setting);
						CameraAnchorPoint->CameraLimit = Limit;
						DTPawn->ChangeCamera(CameraAnchorPoint->CameraTag, Setting, Limit);
					}
				}
			}
		}
	}

}

TArray<FName> UDTCommonFunLibrary::GetAllCameraTag(UObject* WorldContextObject)
{
	TArray<FName> TagArr;
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ACameraAnchorPoint> It(World); It; ++It)
		{
			ACameraAnchorPoint* CameraAnchorPoint = *It;
			if (CameraAnchorPoint && CameraAnchorPoint->IsValidLowLevel())
			{
				TagArr.Add(CameraAnchorPoint->CameraTag);
			}
		}
	}
	return TagArr;
}

void UDTCommonFunLibrary::GetCameraAttibuteByTag(UObject* WorldContextObject, FName InCameraTag, FName InPreCameraTag, FCameraSetting& Setting, FCameraLimit& Limit)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ACameraAnchorPoint> It(World); It; ++It)
		{
			ACameraAnchorPoint* CameraAnchorPoint = *It;
			if (CameraAnchorPoint && CameraAnchorPoint->IsValidLowLevel())
			{
				if (CameraAnchorPoint->CameraTag == InCameraTag)
				{
					Setting = CameraAnchorPoint->CameraSetting;
					Limit = CameraAnchorPoint->CameraLimit;
					APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
					if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
					{
						DTPawn->ChangeCamera(InPreCameraTag, CameraAnchorPoint->CameraSetting, CameraAnchorPoint->CameraLimit);
						break;
					}
				}
			}
		}
	}
}

void UDTCommonFunLibrary::GetCameraTypeByTag(UObject* WorldContextObject, FName InCameraTag, ECameraType& Type)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ACameraAnchorPoint> It(World); It; ++It)
		{
			ACameraAnchorPoint* CameraAnchorPoint = *It;
			if (CameraAnchorPoint && CameraAnchorPoint->IsValidLowLevel())
			{
				if (CameraAnchorPoint->CameraTag == InCameraTag)
				{
					Type= CameraAnchorPoint->CameraType;
					return;
				}
			}
		}
	}
	Type = ECameraType::Common;
}

void UDTCommonFunLibrary::DebugRefreshRange(UObject* WorldContextObject)
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
	if (ADTPawn* DTPawn = Cast<ADTPawn>(Pawn))
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			for (TActorIterator<ACameraAnchorPoint> It(World); It; ++It)
			{
				ACameraAnchorPoint* CameraAnchorPoint = *It;
				if (CameraAnchorPoint && CameraAnchorPoint->IsValidLowLevel())
				{
					if (CameraAnchorPoint->CameraTag == DTPawn->CameraTag)
					{
						CameraAnchorPoint->DebugRefreshRange();
						break;
					}
				}
			}
		}
	}
}


bool UDTCommonFunLibrary::CanMatch(const FString& pattern, const FString& input)
{
	const FRegexPattern frp = FRegexPattern(pattern);
	FRegexMatcher frm = FRegexMatcher(frp, input);
	bool isMatch = frm.FindNext();
	return isMatch;
}

FString UDTCommonFunLibrary::GetLevelName(const AActor* InActor)
{
	return 	InActor->GetLevel()->GetPackage()->GetName();
}

bool UDTCommonFunLibrary::GetWorldPosByMouse(UObject* WorldContextObject, FVector& OutPos)
{
	FVector start = FVector::Zero();
	FVector dir = FVector::Zero();
	FVector end = FVector::Zero();
	if (UGameplayStatics::GetPlayerController(WorldContextObject, 0)->DeprojectMousePositionToWorld(start, dir))
	{
		end = dir * 1000000.0 + start;
	}
	const TArray<AActor*> ignoreArray;
	FHitResult Result;
	bool returnValue = UKismetSystemLibrary::LineTraceSingle(WorldContextObject, start, end, ETraceTypeQuery::TraceTypeQuery1, true, ignoreArray, EDrawDebugTrace::None, Result, true);
	if (returnValue)
	{
		OutPos = Result.ImpactPoint;
		return returnValue;
		//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, Result.Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s LineTraceSingle is not hit something"), *FString(__FUNCTION__));
		return returnValue;
	}
}

AActor* UDTCommonFunLibrary::GetActorByMouse(UObject* WorldContextObject)
{
	FVector start = FVector::Zero();
	FVector dir = FVector::Zero();
	FVector end = FVector::Zero();
	if (UGameplayStatics::GetPlayerController(WorldContextObject, 0)->DeprojectMousePositionToWorld(start, dir))
	{
		end = dir * 1000000.0 + start;
	}
	const TArray<AActor*> ignoreArray;
	FHitResult Result;
	bool returnValue = UKismetSystemLibrary::LineTraceSingle(WorldContextObject, start, end, ETraceTypeQuery::TraceTypeQuery1, true, ignoreArray, EDrawDebugTrace::None, Result, true);
	if (returnValue)
	{
		return Result.GetActor();
		//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, Result.Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s LineTraceSingle is not hit something"), *FString(__FUNCTION__));
		return nullptr;
	}
}

FHitResult UDTCommonFunLibrary::GetHitResultByLineTrace(UObject* WorldContextObject)
{
	FVector start = FVector::Zero();
	FVector dir = FVector::Zero();
	FVector end = FVector::Zero();
	if (UGameplayStatics::GetPlayerController(WorldContextObject, 0)->DeprojectMousePositionToWorld(start, dir))
	{
		end = dir * 1000000.0 + start;
	}
	const TArray<AActor*> ignoreArray;
	FHitResult Result;
	bool returnValue = UKismetSystemLibrary::LineTraceSingle(WorldContextObject, start, end, ETraceTypeQuery::TraceTypeQuery1, true, ignoreArray, EDrawDebugTrace::None, Result, true);
	if (returnValue)
	{
		return Result;
		//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, Result.Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s LineTraceSingle is not hit something"), *FString(__FUNCTION__));
		return Result;
	}
}

class ULineBatchComponent* UDTCommonFunLibrary::GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground)
{
	return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : nullptr);
}

float UDTCommonFunLibrary::GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent)
{
	return bPersistent ? -1.0f : ((LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime);
}

void UDTCommonFunLibrary::DrawDebugLineWithShipping(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriority, const float Thickness, const float LifeTime)
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

void UDTCommonFunLibrary::DrawDebugSphereWithShipping(const UObject* WorldContextObject, FVector const& Center, float Radius, int32 Segments, const FLinearColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
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

int64 UDTCommonFunLibrary::GetTimestamp()
{
	FDateTime Time = FDateTime::Now();

	int64 Timestamp = Time.ToUnixTimestamp();

	return Timestamp;
}

FDateTime UDTCommonFunLibrary::GetTimeFromstamp(int64 UnixTime)
{
	FDateTime Time = FDateTime::FromUnixTimestamp(UnixTime);
	return Time;
}

int32 UDTCommonFunLibrary::ZellerCongruence(FDateTime Time)
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

void UDTCommonFunLibrary::SortIntArray(TArray<int32> InArray, TArray<int32>& OutArray, const bool Ascending)
{
	InArray.Sort([=](int32 A, int32 B) {
		if (Ascending)
		{
			return	A > B;
		}
		else
			return	A < B;
		});
	OutArray = InArray;

}

bool UDTCommonFunLibrary::SpliteMapName(const FString& InMapName, FString& OutBuildName, FString& OutFloorName)
{
	FString Left, Right;
	if (InMapName.Split(FString("_"), &Left, &Right))
	{
		OutBuildName = Left;
		OutFloorName = Right;
		return true;
	}

	return false;
}

bool UDTCommonFunLibrary::GetBuildFloorID(const FString& InMapName, int32& OutBuildID, int32& OutFloorID)
{
	FString Left, Right;
	const TArray<FString> NineFloorArray = { "B1","F1","F2" ,"F3" ,"F4" ,"F5" ,"F6" ,"F7","F8","F9" ,"F10" ,"F11" ,"F12" ,"F13" ,"F14","TOP","RF" };
	const TArray<FString> TenFloorArray = { "B3","B2","B1","F1","1B","F2" ,"F3" ,"F4" ,"4B" ,"F5" ,"F6" ,"6B" ,"F7","F8","F9" ,"F10" ,"F11" ,"F12" ,"F13" ,"F14","F15","F16","F17","F18","F19","F20","TOP"};
	if (InMapName.Split(FString("_"), &Left, &Right))
	{
		const int32 buildId = FCString::Atoi(*Left);
		if (buildId == 9)
			OutBuildID = 8;
		else if (buildId == 4)
			OutBuildID = 3;
		else if(buildId == 3)
			OutBuildID = 2;
		else
			OutBuildID = buildId;

		OutFloorID = OutBuildID==10? TenFloorArray.Find(Right): NineFloorArray.Find(Right);
		return true;
	}
	return false;
}

FDateTime UDTCommonFunLibrary::GetNextDay()
{
	FDateTime Today = FDateTime::Now(); // 获取当前日期时间
	FDateTime Tomorrow = Today + FTimespan(1, 0, 0, 0); // 将当前日期时间增加一天
	return Tomorrow;
}


TArray<FString> UDTCommonFunLibrary::RegexMatch(const FString& pattern, const FString& input)
{
	TArray<FString> results;

	const FRegexPattern frp = FRegexPattern(pattern);
	FRegexMatcher frm = FRegexMatcher(frp, input);

	while (frm.FindNext())
	{
		FString match = frm.GetCaptureGroup(0);
		results.Add(match);
	}

	return results;

}
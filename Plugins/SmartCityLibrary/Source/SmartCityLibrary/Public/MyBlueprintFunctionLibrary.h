// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//要用hv里面的第三方库 需要先包含windows.h  (宏定义跟windows冲突了)
//#include "Windows/AllowWindowsPlatformTypes.h"
//#include "Windows/PreWindowsApi.h"
//#include "../ThirdParty/include/hv/http_client.h"
//#include "Windows/PostWindowsApi.h"
//#include "Windows/HideWindowsPlatformTypes.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */


UCLASS()
class SMARTCITYLIBRARY_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary | Load")
		static UObject* GetObjectByPath(UClass* UClassType, const FString& Name);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary | String")
		static FString FormatStringFromFloat(float Value, int32 DecimalLength);


	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
		static FString GetLocalIP();


	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
		static bool IsChineseCharacter(FString InStr);


	UFUNCTION(BlueprintPure, Category = "FunctionLibrary | Math")
		static TArray<int32> DistanceSort(UPARAM(ref)TMap<FVector, int32>& Map, UObject* WorldContnex);


	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
		static TArray<float>BubbleSortForFloatArray(TArray<float>FloatArray);


	//排序float
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary | UIComponent")
		static TMap<FString, float> SortData(TMap<FString, float> Data, bool Ascending = true);

	//排序int32
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary | UIComponent")
		static TMap<FString, int32> SortDataInt32(TMap<FString, int32> Data, bool Ascending = true);

	//蔡勒算星期公式
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary | UIComponent")
		static int32 ZellerCongruence(FDateTime Time);


	static class ULineBatchComponent* GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground);

	static float GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary | DrawLine")
		static void DrawDebugLineWithShipping(const UObject* WorldContextObject,const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriority, const float Thickness, const float LifeTime);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary | DrawLine")
		static void DrawDebugSphereWithShipping(const UObject* WorldContextObject, FVector const& Center, float Radius, int32 Segments, const FLinearColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);
};

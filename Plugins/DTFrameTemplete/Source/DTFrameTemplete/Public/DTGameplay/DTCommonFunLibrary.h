// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DTGameplay/CameraAnchorPoint.h"
#include "Dom/JsonValue.h"
#include "Templates/SharedPointer.h"
#include "DTCommonFunLibrary.generated.h"




/**
 *
 */
UCLASS()
class DTFRAMETEMPLETE_API UDTCommonFunLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCamera")
	static void ChangeCameraByTag(UObject* WorldContextObject, FName InCameraTag,int32 PlayerIndex = 0);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCamera")
	static void ChangeCameraWithDelegate(UObject* WorldContextObject, FName InCameraTag, FCameraChangeComplete CompleteCallBack, int32 PlayerIndex = 0);
	/**
	 * @InLoc:聚焦的位置
	 * @AnimTime:聚焦过程的动画时间
	 * @Distance：距离聚焦位置的远近(单位m)
	 * @bIsUseInstant:是否瞬间聚焦
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCamera")
		static void CameraFocusOnObject(UObject* WorldContextObject, FVector InLoc, float AnimTime = 1.0f, float Distance = 100, bool bIsUseInstant = false, int32 PlayerIndex = 0);
	/**
	* @InLoc:聚焦的位置
	* @Rot:相机旋转值
	* @AnimTime:聚焦过程的动画时间
	* @Distance：距离聚焦位置的远近(单位m)
	* @bIsUseInstant:是否瞬间聚焦
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCamera")
		static void CameraFocusOnObjectFromRot(UObject* WorldContextObject, FVector InLoc, FRotator Rot, float AnimTime = 1.0f, float Distance = 100, bool bIsUseInstant = false ,int32 PlayerIndex = 0);
	/**
	* @InEyeLoc:从InEyeLoc的位置看向InTargetLoc
	* @InTargetLoc:聚焦的位置
	* @AnimTime:聚焦过程的动画时间
	* @bIsUseInstant:是否瞬间聚焦
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCamera")
	static void CameraLookAtRotation(UObject* WorldContextObject, FVector InEyeLoc, FVector InTargetLoc, float AnimTime = 1.0f, bool bIsUseInstant = false);
	/**
	* 以拱桥的路径形状切换相机
	* @InEyeLoc:从InEyeLoc的位置看向InTargetLoc
	* @InTargetLoc:聚焦的位置
	* @AnimTime:聚焦过程的动画时间
	* @degree拱桥最高点的角度
	* @DisScaleVale拱桥最高点的臂长相对于当前相机的臂长倍数
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCamera")
	static void CameraLookAtRotationArchBridge(UObject* WorldContextObject, FVector InEyeLoc, FVector InTargetLoc, float AnimTime = 1.0f, float degree = 80.f, float DisScaleVale = 1.2f, int32 PlayerIndex = 0);

public:

	UFUNCTION(BlueprintCallable, Category = "YTToolChain")
	static void SaveString(const FString& InPath, const FString& InDataArr);
	UFUNCTION(BlueprintCallable, Category = "YTToolChain")
	static void LoadString(const FString& InPath, FString& OutData);
	UFUNCTION(BlueprintCallable, Category = "YTToolChain")
	static void SaveStringArray(const FString& InPath, const TArray<FString>& InDataArr);
	UFUNCTION(BlueprintCallable, Category = "YTToolChain")
	static void LoadStringArray(const FString& InPath, TArray<FString>& OutData);

	UFUNCTION(BlueprintCallable, Category = "YTToolChain")
	static bool AutoParserJson(const FString& InContent, const FString& InStructParamaName, UObject* InObject);
private:
	//解析数据
	static bool ParseJsonData(UStruct* inStruct, void* inDataAddress, TSharedPtr<FJsonObject> inJsonData, FProperty* spProperty);
	static void UpdatePropertyByJson(FProperty* inProperty, void* inDataAddress, const TSharedPtr<FJsonValue>& inJsonValue, FProperty* spProperty);
public:
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "DTCameraSetting")
	static FCameraSetting GetCurrentCameraSetting(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "DTCameraSetting")
	static FCameraLimit GetCurrentCameraLimit(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCameraSetting")
	static void SetCurrentCameraSetting(UObject* WorldContextObject, FCameraSetting Setting, FCameraLimit Limit);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "DTCameraSetting")
	static TArray<FName> GetAllCameraTag(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "DTCameraSetting")
	static void GetCameraAttibuteByTag(UObject* WorldContextObject,FName InCameraTag, FName InPreCameraTag, FCameraSetting& Setting, FCameraLimit& Limit);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "DTCameraSetting")
	static void GetCameraTypeByTag(UObject* WorldContextObject, FName InCameraTag,ECameraType& Type);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "DTCameraSetting")
	static void DebugRefreshRange(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure,meta = (WorldContext = "WorldContextObject"), Category = "String")
	static TArray<FString> RegexMatch(const FString& pattern, const FString& input);
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "String")
	static bool CanMatch(const FString& pattern, const FString& input);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Level")
	static FString GetLevelName(const AActor* InActor);

	//拿到鼠标点击的世界坐标
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CommonLibrary", meta = (WorldContext = "WorldContextObject"))
	static bool GetWorldPosByMouse(UObject* WorldContextObject, FVector& OutPos);
	//拿到鼠标点击的obj
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CommonLibrary", meta = (WorldContext = "WorldContextObject"))
	static AActor* GetActorByMouse(UObject* WorldContextObject);
	//拿到鼠标点击的信息
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CommonLibrary", meta = (WorldContext = "WorldContextObject"))
		static FHitResult GetHitResultByLineTrace(UObject* WorldContextObject);


	//Draw debug line
	static class ULineBatchComponent* GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground);

	static float GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary | DrawLine")
		static void DrawDebugLineWithShipping(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriority, const float Thickness, const float LifeTime);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary | DrawLine")
		static void DrawDebugSphereWithShipping(const UObject* WorldContextObject, FVector const& Center, float Radius, int32 Segments, const FLinearColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);


	//获取时间戳
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "FunctionLibrary | Time")
		static int64 GetTimestamp();
	//从时间戳获取年月日
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FunctionLibrary | Time")
		static FDateTime GetTimeFromstamp(int64 UnixTime);

	//蔡勒算星期公式
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary | Time")
		static int32 ZellerCongruence(FDateTime Time);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FunctionLibrary | Math")
		static void SortIntArray(TArray<int32> InArray , TArray<int32>& OutArray,const bool Ascending = true);
	
	//判断是否运行在pie
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static inline bool bRunPIE(UObject* WorldContextObject)
	{
		return WorldContextObject->GetWorld()->WorldType == EWorldType::PIE;
	};

	//机器人专用
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool SpliteMapName(const FString& InMapName, FString& OutBuildName, FString& OutFloorName);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool GetBuildFloorID(const FString& InMapName, int32& OutBuildID, int32& OutFloorID);


	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FDateTime GetNextDay();
};

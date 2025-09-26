// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RLToolsBPLibrary.generated.h"

/*
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class URLToolsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

		//打开文件管理器
		UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary")
		static bool OpenWindowsMovie(TArray<FString>& FilePath);

	//获得一个bound的4个角的坐标
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RLToolsBPLibrary")
		static void GetBoundVertexVextur2D(const FVector& Origin, const FVector& BoxExtent, FVector2D& LeftUp, FVector2D& LeftDown, FVector2D& RightUp, FVector2D& RightDown);

#if WITH_EDITOR
	//设置静态模型细节面板里面的DistanceFieldResolutionScale
	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary")
		static void SetDfRes(class UStaticMesh* SM, float TargetDFRes);
#endif

	//打开外部exe文件
	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary | EXE")
		static	void OpenExternalApp(const FString& AppPath);
	//关闭外部exe文件
	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary | EXE")
		static	void CloseExternalApp();
	//打开外部exe文件
	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary | EXE")
		static	void ExecuteExternalApp(const FString& AppPath, const FString& Parameters, int32 nShowCmd = 10);
	//关闭外部exe文件
	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary | EXE")
		static	void KillProcess(const FString& ProcessName);
	//显示虚拟键盘
	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary | EXE")
		static  void ShowVirtualKeyboard();

	//Draw debug line
	static class ULineBatchComponent* GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground);

	static float GetDebugLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent);

	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary | DrawLine")
		static void DrawDebugLineWithShipping(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriority, const float Thickness, const float LifeTime);

	UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary | DrawLine")
		static void DrawDebugSphereWithShipping(const UObject* WorldContextObject, FVector const& Center, float Radius, int32 Segments, const FLinearColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);


	//获取时间戳 秒
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RLToolsBPLibrary | Time")
		static int64 GetTimeSecondstamp();
	//获取时间戳 毫秒
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RLToolsBPLibrary | Time")
		static int64 GetTimeMillisecondstamp();
	//从时间戳获取年月日
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RLToolsBPLibrary | Time")
		static FDateTime GetTimeFromstamp(int64 UnixTime);

	//判断是否运行在pie
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "RLToolsBPLibrary")
		static  bool bRunPIE(UObject* WorldContextObject);

	//设备mac地址
	UFUNCTION(BlueprintPure, Category = "RLToolsBPLibrary")
		static  FString GetMachineID();

		//设置窗口的可视性为top 
		UFUNCTION(BlueprintCallable, Category = "RLToolsBPLibrary")
		static void SetWindowTopMost(uint8 bNeedTopMost = 1, int32 x = 0, int32 y = 0, int32 cx = 0, int32 xy = 0);

};

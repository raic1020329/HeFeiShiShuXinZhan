// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CalculatedPolygonBPLibrary.generated.h"

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
class UCalculatedPolygonBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	//拿到鼠标点击的世界坐标
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon", meta = (WorldContext = "WorldContextObject"))
	static bool GetWorldPosByMouse(UObject* WorldContextObject, FVector& OutPos);

	//是否点在多边形内部
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static bool IsPointInPolygon(const FVector2D& TestPoint, const TArray<FVector2D>& PolygonPoints);

	

	//测量多边形面积
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static float MeasurePolyArea(const TArray<FVector3f>& Locs);


	//获取多边形中心点2D
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static bool GetCenterOfVector2D(const TArray<FVector2D>& Points, FVector2D& Center);

	//获取多边形中心点3D
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static bool GetCenterOfVector(const TArray<FVector>& Points, FVector& Center);
	
	//计算任意多边形的面积，顶点按照顺时针或者逆时针方向排列
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static float CalculatePolygonArea(TArray<FVector> inputPoints);

	//水平距离测量
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static float CalculateHorLength(const FVector & StartPoint,const FVector& EndPoint);

	//垂直距离测量
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static float CalculateVerLength(const FVector& StartPoint, const FVector& EndPoint);

	//空间距离测量
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static float CalculateVectorLength(const FVector& StartPoint, const FVector& EndPoint);

	//角度测量
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static float CalculateVectorAngle(const FVector& StartPoint, const FVector& CenterPoint,const FVector& EndPoint);

	//两点之间任意一点
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
	static FVector CalculatePointBetweenTwoPoints(const FVector& StartPoint, const FVector& EndPoint ,float Invalue);

	//多边形的中心
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CalculatedPolygon")
		static FVector2D CalculateCenterOfPolygon(const TArray<FVector2D>& InPointArray);

};

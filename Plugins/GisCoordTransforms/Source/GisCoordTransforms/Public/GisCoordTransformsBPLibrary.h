// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GisCoordTransformsBPLibrary.generated.h"

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


//GIS信息
USTRUCT(BlueprintType)
struct FGisInfo : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "GIS")
		FString Lng = "";
	UPROPERTY(BlueprintReadWrite, Category = "GIS")
		FString Lat = "";
	UPROPERTY(BlueprintReadWrite, Category = "GIS")
		FVector Location = FVector::Zero();
};


/**
 * 1、GIS经纬度坐标系（WGS84，CGCS2000）
 * 支持WGS84(国际标准)  GCJ02(国标，高德，腾讯)  BD09(百度)   CGCS2000(新国标)
 *
 * 2、GIS投影坐标系
 * 投影坐标系，可以通过参考基准点，通过平移，旋转算法来得到UE4的坐标值
 *
 * 3、北京54，西安80的GIS坐标系统很老了。
 *
 * Gis带号
 * 1、我国采用6度分带和3度分带
 *    地形图上公里网横坐标前2位就是带号，例如：1∶5万地形图上的横坐标为18576000，其中18即为带号，293300为纵坐标值。
 * 在中华人民共和国陆地范围内,坐标(Y坐标,8位数,前两位是带号)带号小于等于23的肯定是6度带，大于等于24的肯定是3度带。
 */


UCLASS()
class UGisCoordTransformsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	/**
	 * 函数功能：投影坐标转换ue坐标
	 */
	UFUNCTION(BlueprintCallable, Category = "GIS")
	static void ProjectionTo_UE(const FString& _longitude, const FString& _latitude, const FString& _Height, const FString& _Orilongitude, const FString& _Orilatitude, float& UE_X, float& UE_Y,float& UE_Z);

	/**
	 * 函数功能：Xian80到WGS84转换过程.
	 * @param	str_xian80_X		gis点在Xian80的X值（十进制格式）.
	 * @param	str_xian80_Y		gis点在Xian80的Y值（十进制格式）.
	 * @param	bandNum				Xian80的Y值（十进制格式）.
	 * @param	str_wgs84_lng		gis点在gcj02的longitude值（十进制格式）.
	 * @param	str_wgs84_lng		gis点在gcj02的latitude值（十进制格式）.
	 * @return	void.
	 */
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_Xian80_To_WGS84(const FString& str_xian80_X, const FString& str_xian80_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat);

	/**
	 * 函数功能：Beijing54到WGS84转换过程.
	 * @param	str_beijing54_X		gis点在Beijing54的X值（十进制格式）.
	 * @param	str_beijing54_Y		gis点在Beijing54的Y值（十进制格式）.
	 * @param	bandNum				Xian80的Y值（十进制格式）.
	 * @param	str_wgs84_lng		gis点在gcj02的longitude值（十进制格式）.
	 * @param	str_wgs84_lng		gis点在gcj02的latitude值（十进制格式）.
	 * @return	void.
	 */
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_Beijing54_To_WGS84(const FString& str_beijing54_X, const FString& str_beijing54_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat);


	/**
	 * 函数功能：WGS84到GCJ02转换过程.
	 * @param	str_wgs84_lng		gis点在wgs84的longitude值（十进制格式）.
	 * @param	str_wgs84_lat		gis点在wgs84的latitude值（十进制格式）.
	 * @param	str_gcj02_lng		gis点在gcj02的longitude值（十进制格式）.
	 * @param	str_gcj02_lat		gis点在gcj02的latitude值（十进制格式）.
	 * @return	void.
	 */
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_WGS84_To_GCJ02(const FString& str_wgs84_lng, const FString& str_wgs84_lat, FString& str_gcj02_lng, FString& str_gcj02_lat);

	/**
	* 函数功能：GCJ02到WGS84转换过程.
	* @param	str_gcj02_lng		gis点在gcj02的longitude值（十进制格式）.
	* @param	str_gcj02_lat		gis点在gcj02的latitude值（十进制格式）.
	* @param	str_wgs84_lng		gis点在wgs84的longitude值（十进制格式）.
	* @param	str_wgs84_lat		gis点在wgs84的latitude值（十进制格式）.
	* @return	void.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_GCJ02_To_WGS84(const FString& str_gcj02_lng, const FString& str_gcj02_lat, FString& str_wgs84_lng, FString& str_wgs84_lat);


	/**
	* 函数功能：WGS84到BD09转换过程.
	* @param	str_wgs84_lng		gis点在wgs84的longitude值（十进制格式）.
	* @param	str_wgs84_lat		gis点在wgs84的latitude值（十进制格式）.
	* @param	str_bd09_long		gis点在bd09的longitude值（十进制格式）.
	* @param	str_bd09_lat		gis点在bd09的latitude值（十进制格式）.
	* @return	void.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_WGS84_To_BD09(const FString& str_wgs84_long, const FString& str_wgs84_lat, FString& str_bd09_long, FString& str_bd09_lat);

	/**
	* 函数功能：BD09到WGS84转换过程.
	* @param	str_bd09_long		gis点在bd09的longitude值（十进制格式）.
	* @param	str_bd09_lat		gis点在bd09的latitude值（十进制格式）.
	* @param	str_wgs84_lng		gis点在wgs84的longitude值（十进制格式）.
	* @param	str_wgs84_lat		gis点在wgs84的latitude值（十进制格式）.
	* @return	void.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_BD09_To_WGS84(const FString& str_bd09_long, const FString& str_bd09_lat, FString& str_wgs84_long, FString& str_wgs84_lat);

	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_GCJ02_To_BD09(const FString& str_gcj02_long, const FString& str_gcj02_lat, FString& str_bd09_long, FString& str_bd09_lat);

	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_BD09_To_GCJ02(const FString& str_bd09_long, const FString& str_bd09_lat, FString& str_gcj02_long, FString& str_gcj02_lat);


	/**
	* 函数功能：WGS84到CGCS2000转换过程.(WGS84和CGCS2000的经纬度误差十分小，结果可以认为是相同的）
	* @param	str_wgs84_lng		gis点在wgs84的longitude值（十进制格式）.
	* @param	str_wgs84_lat		gis点在wgs84的latitude值（十进制格式）.
	* @param	str_wgs84_height	gis点在wgs84的height值（十进制格式）.
	* @param	str_cgcs2000_lng		gis点在wgs84的longitude值（十进制格式）.
	* @param	str_cgcs2000_lat		gis点在wgs84的latitude值（十进制格式）.
	* @param	str_cgcs2000_height	gis点在wgs84的height值（十进制格式）.
	* @return	void.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_WGS84_To_CGCS2000(const FString& str_wgs84_lng, const FString& str_wgs84_lat, const FString& str_wgs84_height, FString& str_cgcs2000_lng, FString& str_cgcs2000_lat, FString& str_cgcs2000_height);
	/**
	* 函数功能：CGCS2000到WGS84转换过程.(WGS84和CGCS2000的经纬度误差十分小，结果可以认为是相同的）
	* @param	str_cgcs2000_lng		gis点在wgs84的longitude值（十进制格式）.
	* @param	str_cgcs2000_lat		gis点在wgs84的latitude值（十进制格式）.
	* @param	str_cgcs2000_height	gis点在wgs84的height值（十进制格式）.
	* @param	str_wgs84_lng		gis点在wgs84的longitude值（十进制格式）.
	* @param	str_wgs84_lat		gis点在wgs84的latitude值（十进制格式）.
	* @param	str_wgs84_height	gis点在wgs84的height值（十进制格式）.
	* @return	void.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static void GIS_CGCS2000_To_WGS84(const FString& str_cgcs2000_lng, const FString& str_cgcs2000_lat, const FString& str_cgcs2000_height, FString& str_wgs84_lng, FString& str_wgs84_lat, FString& str_wgs84_height);







	/**
	* 函数功能：WGS坐标到UE4坐标转换过程(单个参考点).
	* @param	Position_Long		gis点在wgs84的longitude值（十进制格式）.
	* @param	Position_Lat		gis点在wgs84的latitude值（十进制格式）.
	* @param	WorldPivot			pivot的ue4水平坐标数据.
	* @param	WorldOrigin_Long	参考pivot的gis坐标lat（十进制格式）.
	* @param	WorldOrigin_Lat		参考pivot的gis坐标lat（十进制格式）.
	* @param	Height				ue4中设定的高度值（十进制格式）.
	* @return	FGisInfo			当前gis点的gis坐标和ue4坐标组.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static FGisInfo GIS_LongLat_To_UE4Coord_OnePivot(
			const FString& Position_Long, //输入的gis坐标long
			const FString& Position_Lat, //输入的gis坐标lat
			const FVector& WorldPivot, //pivot的ue4水平坐标数据
			const FString& WorldOrigin_Long = "119.975706917678", //参考pivot的gis坐标lat
			const FString& WorldOrigin_Lat = "30.523512314383", //参考pivot的gis坐标lat
			float Height = 0.f);//ue4中设定的高度值


	/**
	* 函数功能：UE4坐标到GIS坐标转换过程(单个参考点).
	* @param	WorldPosition		gis点在ue4水平坐标数据.
	* @param	WorldPivot			pivot的ue4水平坐标数据.
	* @param	WorldOrigin_Long	参考pivot的gis坐标lat（十进制格式）.
	* @param	WorldOrigin_Lat		参考pivot的gis坐标lat（十进制格式）.
	* @return	FGisInfo			当前gis点的gis坐标和ue4坐标组.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static FGisInfo GIS_UE4Coord_To_LongLat_OnePivot(
			const FVector& WorldPosition, //输入的ue4水平坐标数据
			const FVector& WorldPivot, //pivot的ue4水平坐标数据
			const FString& WorldOrigin_Long = "116.43859863", //参考pivot的gis坐标lat
			const FString& WorldOrigin_Lat = "39.95080618");//参考pivot的gis坐标lat

	/**
	* 函数功能：WGS坐标到UE4坐标转换过程(多个参考点).
	* @param	Lng					gis点在wgs84的longitude值（十进制格式）.
	* @param	Lat					gis点在wgs84的latitude值（十进制格式）.
	* @param	PivotArray			pivot组的ue4水平坐标数据.
	* @param	Height				ue4中设定的高度值（十进制格式）.
	* @return	FGisInfo			当前gis点的gis坐标和ue4坐标组.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static FGisInfo GIS_LongLat_To_UE4Coord_MultiPivots(
			const FString& Lng, //输入的gis坐标long
			const FString& Lat, //输入的gis坐标lat
			const TArray<FGisInfo>& PivotArray, //pivot1的ue4水平坐标数据
			float Height = 0.0f);//ue4中设定的高度值


	/**
	* 函数功能：UE4坐标到GIS坐标转换过程(多个参考点).
	* @param	Loc					gis点在ue4水平坐标数据.
	* @param	PivotArray			pivot组的ue4水平坐标数据.
	* @return	FGisInfo			当前gis点的gis坐标和ue4坐标组.
	*/
	UFUNCTION(BlueprintCallable, Category = "GIS")
		static FGisInfo GIS_UE4Coord_To_LongLat_MultiPivots(
			const FVector& Loc,
			const TArray<FGisInfo>& PivotArray); //pivot的ue4水平坐标数据
};

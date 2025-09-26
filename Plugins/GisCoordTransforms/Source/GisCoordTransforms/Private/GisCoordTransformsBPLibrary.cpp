// Copyright Epic Games, Inc. All Rights Reserved.

#include "GisCoordTransformsBPLibrary.h"
#include "GisCoordTransforms.h"
#include "GPSUtil.h"
#include "LatLong_UTMconversion.h"

UGisCoordTransformsBPLibrary::UGisCoordTransformsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


void UGisCoordTransformsBPLibrary::ProjectionTo_UE(const FString& _longitude, const FString& _latitude, const FString& _Height, const FString& _Orilongitude, const FString& _Orilatitude, float& UE_X, float& UE_Y, float& UE_Z)
{
	UE_X = (FCString::Atof(*_longitude) - FCString::Atof(*_Orilongitude)) * 100.0f;
	UE_Y = (FCString::Atof(*_latitude) - FCString::Atof(*_Orilatitude)) * -100.0f;
	UE_Z = FCString::Atof(*_Height) * 100.0f;
}

void UGisCoordTransformsBPLibrary::GIS_Xian80_To_WGS84(const FString& str_xian80_X, const FString& str_xian80_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat)
{
	FGPSUtil::GIS_Xian80_To_WGS84(str_xian80_X, str_xian80_Y, bandNum, str_wgs84_lng, str_wgs84_lat);
}

void UGisCoordTransformsBPLibrary::GIS_Beijing54_To_WGS84(const FString& str_beijing54_X, const FString& str_beijing54_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat)
{
	FGPSUtil::GIS_Beijing54_To_WGS84(str_beijing54_X, str_beijing54_Y, bandNum, str_wgs84_lng, str_wgs84_lat);
}



void UGisCoordTransformsBPLibrary::GIS_WGS84_To_GCJ02(const FString& str_wgs84_lng, const FString& str_wgs84_lat, FString& str_gcj02_lng, FString& str_gcj02_lat)
{
	double wgs84_lng = FCString::Atod(*str_wgs84_lng);
	double wgs84_lat = FCString::Atod(*str_wgs84_lat);
	double gcj02_lng;
	double gcj02_lat;

	FGPSUtil::WGS84_To_GCJ02(wgs84_lng, wgs84_lat, gcj02_lng, gcj02_lat);

	str_gcj02_lat = FString::SanitizeFloat(gcj02_lat);
	str_gcj02_lng = FString::SanitizeFloat(gcj02_lng);

}




void UGisCoordTransformsBPLibrary::GIS_GCJ02_To_WGS84(const FString& str_gcj02_lng, const FString& str_gcj02_lat, FString& str_wgs84_lng, FString& str_wgs84_lat)
{
	double wgs84_lng;
	double wgs84_lat;
	double gcj02_lng = FCString::Atod(*str_gcj02_lng);
	double gcj02_lat = FCString::Atod(*str_gcj02_lat);

	FGPSUtil::GCJ02_To_WGS84(gcj02_lng, gcj02_lat, wgs84_lng, wgs84_lat);

	str_wgs84_lat = FString::SanitizeFloat(wgs84_lat);
	str_wgs84_lng = FString::SanitizeFloat(wgs84_lng);
}

void UGisCoordTransformsBPLibrary::GIS_WGS84_To_BD09(const FString& str_wgs84_long, const FString& str_wgs84_lat, FString& str_bd09_long, FString& str_bd09_lat)
{
	FString str_gcj02_long;
	FString str_gcj02_lat;
	//step1
	UGisCoordTransformsBPLibrary::GIS_WGS84_To_GCJ02(str_wgs84_long, str_wgs84_lat, str_gcj02_long, str_gcj02_lat);
	//step2
	UGisCoordTransformsBPLibrary::GIS_GCJ02_To_BD09(str_gcj02_long, str_gcj02_lat, str_bd09_long, str_bd09_lat);
}

void UGisCoordTransformsBPLibrary::GIS_BD09_To_WGS84(const FString& str_bd09_long, const FString& str_bd09_lat, FString& str_wgs84_long, FString& str_wgs84_lat)
{
	FString str_gcj02_long;
	FString str_gcj02_lat;
	//step1
	UGisCoordTransformsBPLibrary::GIS_BD09_To_GCJ02(str_bd09_long, str_bd09_lat, str_gcj02_long, str_gcj02_lat);
	//step2
	UGisCoordTransformsBPLibrary::GIS_GCJ02_To_WGS84(str_gcj02_long, str_gcj02_lat, str_wgs84_long, str_wgs84_lat);
}




//************************************
// Method:    GCJ02_To_BD09                           -火星坐标（高德、谷歌中国）转百度坐标
// FullName:  UFunctionLibraryBPLibrary::GCJ02_To_BD09
// Access:    public static 
// Parameter: FString str_gcj02_long          - 输入火星经度
// Parameter: FString str_gcj02_lat             - 输入火星纬度
// Parameter: FString & str_bd09_long      - 返回百度经度
// Parameter: FString & str_bd09_lat         - 返回百度纬度
//************************************
void UGisCoordTransformsBPLibrary::GIS_GCJ02_To_BD09(const FString& str_gcj02_long, const FString& str_gcj02_lat, FString& str_bd09_long, FString& str_bd09_lat)
{
	double gcj02_long = FCString::Atod(*str_gcj02_long);
	double gcj02_lat = FCString::Atod(*str_gcj02_lat);
	double bd09_long;
	double bd09_lat;

	FGPSUtil::GCJ02_To_BD09(gcj02_long, gcj02_lat, bd09_long, bd09_lat);

	str_bd09_long = FString::SanitizeFloat(bd09_long);
	str_bd09_lat = FString::SanitizeFloat(bd09_lat);
}

void UGisCoordTransformsBPLibrary::GIS_BD09_To_GCJ02(const FString& str_bd09_long, const FString& str_bd09_lat, FString& str_gcj02_long, FString& str_gcj02_lat)
{
	double bd09_long = FCString::Atod(*str_bd09_long);
	double bd09_lat = FCString::Atod(*str_bd09_lat);
	double gcj02_long;
	double gcj02_lat;

	FGPSUtil::BD09_To_GCJ02(bd09_long, bd09_lat, gcj02_long, gcj02_lat);

	str_gcj02_long = FString::SanitizeFloat(gcj02_long);
	str_gcj02_lat = FString::SanitizeFloat(gcj02_lat);
}





void UGisCoordTransformsBPLibrary::GIS_WGS84_To_CGCS2000(const FString& str_wgs84_lng, const FString& str_wgs84_lat, const FString& str_wgs84_height, FString& str_cgcs2000_lng, FString& str_cgcs2000_lat, FString& str_cgcs2000_height)
{
	FGPSUtil::WGS84_To_CGCS2000(str_wgs84_lng, str_wgs84_lat, str_wgs84_height, str_cgcs2000_lng, str_cgcs2000_lat, str_cgcs2000_height);
}

void UGisCoordTransformsBPLibrary::GIS_CGCS2000_To_WGS84(const FString& str_cgcs2000_lng, const FString& str_cgcs2000_lat, const FString& str_cgcs2000_height, FString& str_wgs84_lng, FString& str_wgs84_lat, FString& str_wgs84_height)
{
	FGPSUtil::CGCS2000_To_WGS84(str_cgcs2000_lng, str_cgcs2000_lat, str_cgcs2000_height, str_wgs84_lng, str_wgs84_lat, str_wgs84_height);
}







FGisInfo UGisCoordTransformsBPLibrary::GIS_LongLat_To_UE4Coord_OnePivot(
	const FString& Position_Long, /*输入的gis坐标long */
	const FString& Position_Lat, /*输入的gis坐标lat */
	const FVector& WorldPivot, /*pivot的ue4水平坐标数据 */ 
	const FString& WorldOrigin_Long /*= "116.43859863"*/, /*参考pivot的gis坐标lat */
	const FString& WorldOrigin_Lat /*= "39.95080618"*/, /*参考pivot的gis坐标lat */
	float Height /*= 0.f*/)
{
	double InputPosition_Lont_ = FCString::Atod(*Position_Long);
	double InputPosition_Lat_ = FCString::Atod(*Position_Lat);
	double WorldOrigin_Long_ = FCString::Atod(*WorldOrigin_Long);
	double WorldOrigin_Lat_ = FCString::Atod(*WorldOrigin_Lat);
	double Position_X, Position_Y;

	Position_X = InputPosition_Lont_;
	Position_Y = InputPosition_Lat_;
	LLtoTMERC(WorldOrigin_Long_, WorldOrigin_Lat_, Position_X, Position_Y);


	FGisInfo gisValue;
	gisValue.Lng = Position_Long;
	gisValue.Lat = Position_Lat;
	gisValue.Location = FVector(WorldPivot.X + (Position_X) * 100.0f, WorldPivot.Y - (Position_Y) * 100.0f, Height);


	return gisValue;
}

FGisInfo UGisCoordTransformsBPLibrary::GIS_UE4Coord_To_LongLat_OnePivot(
	const FVector& WorldPosition, /*输入的ue4水平坐标数据 */ 
	const FVector& WorldPivot, /*pivot的ue4水平坐标数据 */ 
	const FString& WorldOrigin_Long /*= "116.43859863"*/, /*参考pivot的gis坐标lat */ 
	const FString& WorldOrigin_Lat /*= "39.95080618"*/)
{
	double WorldOrigin_Long_ = FCString::Atod(*WorldOrigin_Long);
	double WorldOrigin_Lat_ = FCString::Atod(*WorldOrigin_Lat);

	FVector offset2D = WorldPosition - WorldPivot;


	double Position_X = offset2D.X / 100.0f;
	double Position_Y = -offset2D.Y / 100.0f;

	TMERCtoLL(WorldOrigin_Long_, WorldOrigin_Lat_, Position_X, Position_Y);

	FString Lat = FString::SanitizeFloat(Position_Y);
	FString Long = FString::SanitizeFloat(Position_X);

	FGisInfo gisValue;
	gisValue.Lng = Long;
	gisValue.Lat = Lat;
	gisValue.Location = WorldPosition;


	return gisValue;
}

FGisInfo UGisCoordTransformsBPLibrary::GIS_LongLat_To_UE4Coord_MultiPivots(
	const FString& Lng, /*输入的gis坐标long */
	const FString& Lat, /*输入的gis坐标lat */
	const TArray<FGisInfo>& PivotArray, /*pivot1的ue4水平坐标数据 */ 
	float Height /*= 0.0f*/)
{
	int32 PovitNum = PivotArray.Num();
	TArray<FVector> coordList;

	for (int32 i = 0; i < PovitNum; ++i)
	{
		FGisInfo gi = PivotArray[i];
		FGisInfo newPos = UGisCoordTransformsBPLibrary::GIS_LongLat_To_UE4Coord_OnePivot(Lng, Lat, gi.Location, gi.Lng, gi.Lat, Height);
		coordList.Add(newPos.Location);
	}

	FVector sumV = FVector::ZeroVector;
	for (int32 j = 0; j < PovitNum; ++j)
	{
		sumV += coordList[j];
	}

	FGisInfo targetGis;
	targetGis.Location = sumV / PovitNum;
	targetGis.Lng = Lng;
	targetGis.Lat = Lat;

	return targetGis;
}

FGisInfo UGisCoordTransformsBPLibrary::GIS_UE4Coord_To_LongLat_MultiPivots(const FVector& Loc, const TArray<FGisInfo>& PivotArray)
{
	int32 PovitNum = PivotArray.Num();
	TArray<double> longList;
	TArray<double> latList;

	for (int32 i = 0; i < PovitNum; ++i)
	{
		FGisInfo gi = PivotArray[i];
		FString Lng;
		FString Lat;

		FGisInfo newGisValue = UGisCoordTransformsBPLibrary::GIS_UE4Coord_To_LongLat_OnePivot(Loc, gi.Location, gi.Lng, gi.Lat);

		double WorldOrigin_Long_ = FCString::Atod(*newGisValue.Lng);
		double WorldOrigin_Lat_ = FCString::Atod(*newGisValue.Lat);
		longList.Add(WorldOrigin_Long_);
		latList.Add(WorldOrigin_Lat_);
	}

	double pointLong = 0;
	double pointLat = 0;
	for (int32 idx = 0; idx < longList.Num(); ++idx)
	{
		pointLong += longList[idx];
		pointLat += latList[idx];
	}

	pointLong /= longList.Num();
	pointLat /= longList.Num();

	FGisInfo targetGis;
	targetGis.Location = Loc;
	targetGis.Lng = FString::SanitizeFloat(pointLong);
	targetGis.Lat = FString::SanitizeFloat(pointLat);

	return targetGis;
}


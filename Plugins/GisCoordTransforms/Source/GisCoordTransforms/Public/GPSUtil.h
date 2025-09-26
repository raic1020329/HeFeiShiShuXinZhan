// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
* 国际标准坐标系(WGS84), 百度坐标系 (BD-09), 火星坐标系 (GCJ-02, 高德)的转换
*/
class FGPSUtil
{
private:
	static const double G_PI;
	static const double EARTH_R;
	static const double EARTH_EE;

	static const double BaseGisLng;
	static const double BaseGisLat;

	static bool OutOfChina(double lng, double lat);
	static double TransformLat(double lng, double lat);
	static double TransformLng(double lng, double lat);
	static void GaussProjInvCal(double X, double Y, double daihao, double a, double f, double& longitude, double& latitude);

public:
	static void GIS_Xian80_To_WGS84(FString str_xian80_X, FString str_xian80_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat);
	static void GIS_Beijing54_To_WGS84(FString str_beijing54_X, FString str_beijing54_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat);

	static void WGS84_To_GCJ02(double lng, double lat, double& lngGCJ02, double& latGCJ02);
	static void GCJ02_To_WGS84(double lng, double lat, double& lngWSG84, double& latWSG84);

	static void GCJ02_To_BD09(double lng, double lat, double& lngBD09, double& latBD09);
	static void BD09_To_GCJ02(double bd_lon, double bd_lat, double& lngGCJ02, double& latGCJ02);

	static void WGS84_To_CGCS2000(FString str_wgs84_lng, FString str_wgs84_lat, FString str_wgs84_height, FString& str_cgcs2000_lng, FString& str_cgcs2000_lat, FString& str_cgcs2000_height);
	static void CGCS2000_To_WGS84(FString str_cgcs2000_lng, FString str_cgcs2000_lat, FString str_cgcs2000_height, FString& str_wgs84_lng, FString& str_wgs84_lat, FString& str_wgs84_height);
};

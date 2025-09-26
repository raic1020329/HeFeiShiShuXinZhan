// Fill out your copyright notice in the Description page of Project Settings.


#include "GPSUtil.h"


const double FGPSUtil::G_PI = 3.14159265358979324;
const double FGPSUtil::EARTH_R = 6378245.0;
const double FGPSUtil::EARTH_EE = 0.00669342162296594323;

const double FGPSUtil::BaseGisLng = 105.0;
const double FGPSUtil::BaseGisLat = 35.0;


bool FGPSUtil::OutOfChina(double lng, double lat)
{
	if (lng < 72.004 || lng > 137.8347)
		return true;
	if (lat < 0.8293 || lat > 55.8271)
		return true;

	return false;
}

double FGPSUtil::TransformLat(double lng, double lat)
{
	double ret;
	ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat + 0.1 * lng * lat + 0.2 * FMath::Sqrt(FMath::Abs(lng));
	ret += (20.0 * FMath::Sin(6.0 * lng * G_PI) + 20.0 * FMath::Sin(2.0 * lng * G_PI)) * 2.0 / 3.0;
	ret += (20.0 * FMath::Sin(lat * G_PI) + 40.0 * FMath::Sin(lat / 3.0 * G_PI)) * 2.0 / 3.0;
	ret += (160.0 * FMath::Sin(lat / 12.0 * G_PI) + 320 * FMath::Sin(lat * G_PI / 30.0)) * 2.0 / 3.0;
	return ret;
}

double FGPSUtil::TransformLng(double lng, double lat)
{
	double ret;
	ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng + 0.1 * lng * lat + 0.1 * FMath::Sqrt(FMath::Abs(lng));
	ret += (20.0 * FMath::Sin(6.0 * lng * G_PI) + 20.0 * FMath::Sin(2.0 * lng * G_PI)) * 2.0 / 3.0;
	ret += (20.0 * FMath::Sin(lng * G_PI) + 40.0 * FMath::Sin(lng / 3.0 * G_PI)) * 2.0 / 3.0;
	ret += (150.0 * FMath::Sin(lng / 12.0 * G_PI) + 300.0 * FMath::Sin(lng / 30.0 * G_PI)) * 2.0 / 3.0;
	return ret;
}

void FGPSUtil::GaussProjInvCal(double X, double Y, double daihao, double a, double f, double& longitude, double& latitude)
{
	int ProjNo; ////带宽
	double longitude1, latitude1, longitude0, X0, Y0, xval, yval;
	double e1, e2, ee, NN, T, C, M, D, R, u, fai, iPI;
	iPI = 0.0174532925199433; ////3.1415926535898/180.0;
	////a = 6378245.0; f = 1.0 / 298.3; //54年北京坐标系参数
	////a=6378140.0; f=1/298.257; //80年西安坐标系参数
	ProjNo = (int)(X / 1000000L); //查找带号
	longitude0 = daihao * 3;
	longitude0 = longitude0 * iPI; //中央经线
	X0 = ProjNo * 1000000L + 500000L;
	Y0 = 0;
	xval = X - X0; yval = Y - Y0; //带内大地坐标
	e2 = 2 * f - f * f;
	e1 = (1.0 - FMath::Sqrt(1 - e2)) / (1.0 + FMath::Sqrt(1 - e2));
	ee = e2 / (1 - e2);
	M = yval;
	u = M / (a * (1 - e2 / 4 - 3 * e2 * e2 / 64 - 5 * e2 * e2 * e2 / 256));
	fai = u + (3 * e1 / 2 - 27 * e1 * e1 * e1 / 32) * FMath::Sin(2 * u) + (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32) * FMath::Sin(
		4 * u)
		+ (151 * e1 * e1 * e1 / 96) * FMath::Sin(6 * u) + (1097 * e1 * e1 * e1 * e1 / 512) * FMath::Sin(8 * u);
	C = ee * FMath::Cos(fai) * FMath::Cos(fai);
	T = FMath::Tan(fai) * FMath::Tan(fai);
	NN = a / FMath::Sqrt(1.0 - e2 * FMath::Sin(fai) * FMath::Sin(fai));
	R = a * (1 - e2) / FMath::Sqrt((1 - e2 * FMath::Sin(fai) * FMath::Sin(fai)) * (1 - e2 * FMath::Sin(fai) * FMath::Sin(fai)) * (1 - e2 * FMath::Sin
	(fai) *FMath::Sin(fai)));
	D = xval / NN;
	//计算经度(Longitude) 纬度(Latitude)
	longitude1 = longitude0 + (D - (1 + 2 * T + C) * D * D * D / 6 + (5 - 2 * C + 28 * T - 3 * C * C + 8 * ee + 24 * T * T) * D
		* D * D * D * D / 120) / FMath::Cos(fai);
	latitude1 = fai - (NN * FMath::Tan(fai) / R) * (D * D / 2 - (5 + 3 * T + 10 * C - 4 * C * C - 9 * ee) * D * D * D * D / 24
		+ (61 + 90 * T + 298 * C + 45 * T * T - 256 * ee - 3 * C * C) * D * D * D * D * D * D / 720);
	//转换为度 DD
	longitude = longitude1 / iPI;
	latitude = latitude1 / iPI;
}



void FGPSUtil::GIS_Xian80_To_WGS84(FString str_xian80_X, FString str_xian80_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat)
{
	double a = 6378140.0; double f = 1 / 298.257; //80年西安坐标系参数
	double x = FCString::Atod(*str_xian80_X);
	double y = FCString::Atod(*str_xian80_Y);
	double wgs84lng = 0;
	double wgs84lat = 0;
	FGPSUtil::GaussProjInvCal(x, y, bandNum, a, f, wgs84lng, wgs84lat);

	str_wgs84_lng = FString::SanitizeFloat(wgs84lng);
	str_wgs84_lat = FString::SanitizeFloat(wgs84lat);
}

void FGPSUtil::GIS_Beijing54_To_WGS84(FString str_beijing54_X, FString str_beijing54_Y, int32 bandNum, FString& str_wgs84_lng, FString& str_wgs84_lat)
{
	double a = 6378245.0; double f = 1.0 / 298.3; //54年北京坐标系参数
	double x = FCString::Atod(*str_beijing54_X);
	double y = FCString::Atod(*str_beijing54_Y);
	double wgs84lng = 0;
	double wgs84lat = 0;
	FGPSUtil::GaussProjInvCal(x, y, bandNum, a, f, wgs84lng, wgs84lat);

	str_wgs84_lng = FString::SanitizeFloat(wgs84lng);
	str_wgs84_lat = FString::SanitizeFloat(wgs84lat);
}



/**
 * WGS84转GCJ02(火星坐标系)
 *
 * @param lng WGS84坐标系的经度
 * @param lat WGS84坐标系的纬度
 * @return 火星坐标数组
 */
void FGPSUtil::WGS84_To_GCJ02(double lng, double lat, double& lngGCJ02, double& latGCJ02)
{
	if (OutOfChina(lng, lat))
	{
		lngGCJ02 = lng;
		latGCJ02 = lat;

		return;
	}

	double dLat = TransformLat(lng - BaseGisLng, lat - BaseGisLat);
	double dLon = TransformLng(lng - BaseGisLng, lat - BaseGisLat);
	double radLat = lat / 180.0 * G_PI;
	double magic = FMath::Sin(radLat);
	magic = 1 - EARTH_EE * magic * magic;

	double sqrtMagic = FMath::Sqrt(magic);
	dLat = (dLat * 180.0) / ((EARTH_R * (1 - EARTH_EE)) / (magic * sqrtMagic) * G_PI);
	dLon = (dLon * 180.0) / (EARTH_R / sqrtMagic * FMath::Cos(radLat) * G_PI);

	latGCJ02 = lat + dLat;
	lngGCJ02 = lng + dLon;
}

/**
	 * GCJ02(火星坐标系)转GPS84
	 *
	 * @param lng 火星坐标系的经度
	 * @param lat 火星坐标系纬度
	 * @return WGS84坐标数组
	 */
void FGPSUtil::GCJ02_To_WGS84(double lng, double lat, double& lngWSG84, double& latWSG84)
{
	if (OutOfChina(lng, lat)) {
		lngWSG84 = lng;
		latWSG84 = lat;
		return;
	}

	//Latitude
	double dlat = TransformLat(lng - BaseGisLng, lat - BaseGisLat);
	double radlat = lat / 180.0 * G_PI;
	double magic = FMath::Sin(radlat);
	magic = 1 - EARTH_EE * magic * magic;
	double sqrtmagic = FMath::Sqrt(magic);
	dlat = (dlat * 180.0) / ((EARTH_R * (1 - EARTH_EE)) / (magic * sqrtmagic) * G_PI);
	latWSG84 = lat - dlat;


	//Longitude
	double dlng = TransformLng(lng - BaseGisLng, lat - BaseGisLat);
	dlng = (dlng * 180.0) / (EARTH_R / sqrtmagic * FMath::Cos(radlat) * G_PI);
	lngWSG84 = lng - dlng;
}





/**
   * 火星坐标系 (GCJ-02) 与百度坐标系 (BD-09) 的转换
   * 即谷歌、高德 转 百度
   * @param lng
   * @param lat     gcj02tobd09
   * @return
   * @returns {*[]}
   */
void FGPSUtil::GCJ02_To_BD09(double lng, double lat, double& lngBD09, double& latBD09)
{
	double z = FMath::Sqrt(lng * lng + lat * lat) + 0.00002 * FMath::Sin(lat * G_PI);
	double theta = FMath::Atan2(lat, lng) + 0.000003 * FMath::Cos(lng * G_PI);
	lngBD09 = z * FMath::Cos(theta) + 0.0065;
	latBD09 = z * FMath::Sin(theta) + 0.006;
}


/**
	   * 百度坐标系 (BD-09) 与 火星坐标系 (GCJ-02)的转换
	   * 即 百度 转 谷歌、高德
	   * @param bd_lon
	   * @param bd_lat
	   * @returns {*[]}
	   */
void FGPSUtil::BD09_To_GCJ02(double bd_lon, double bd_lat, double& lngGCJ02, double& latGCJ02)
{
	double x = bd_lon - 0.0065;
	double y = bd_lat - 0.006;
	double z = FMath::Sqrt(x * x + y * y) - 0.00002 * FMath::Sin(y * G_PI);
	double theta = FMath::Atan2(y, x) - 0.000003 * FMath::Cos(x * G_PI);
	lngGCJ02 = z * FMath::Cos(theta);
	latGCJ02 = z * FMath::Sin(theta);
}



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
void FGPSUtil::WGS84_To_CGCS2000(FString str_wgs84_lng, FString str_wgs84_lat, FString str_wgs84_height, FString& str_cgcs2000_lng, FString& str_cgcs2000_lat, FString& str_cgcs2000_height)
{
	str_cgcs2000_lng = str_wgs84_lng;
	str_cgcs2000_lat = str_wgs84_lat;
	str_cgcs2000_height = str_wgs84_height;
}

/**
* 函数功能：WGS84到CGCS2000转换过程.(WGS84和CGCS2000的经纬度误差十分小，结果可以认为是相同的）
* @param	str_cgcs2000_lng		gis点在wgs84的longitude值（十进制格式）.
* @param	str_cgcs2000_lat		gis点在wgs84的latitude值（十进制格式）.
* @param	str_cgcs2000_height	gis点在wgs84的height值（十进制格式）.
* @param	str_wgs84_lng		gis点在wgs84的longitude值（十进制格式）.
* @param	str_wgs84_lat		gis点在wgs84的latitude值（十进制格式）.
* @param	str_wgs84_height	gis点在wgs84的height值（十进制格式）.
* @return	void.
*/
void FGPSUtil::CGCS2000_To_WGS84(FString str_cgcs2000_lng, FString str_cgcs2000_lat, FString str_cgcs2000_height, FString& str_wgs84_lng, FString& str_wgs84_lat, FString& str_wgs84_height)
{
	str_wgs84_lng = str_cgcs2000_lng;
	str_wgs84_lat = str_cgcs2000_lat;
	str_wgs84_height = str_cgcs2000_height;
}

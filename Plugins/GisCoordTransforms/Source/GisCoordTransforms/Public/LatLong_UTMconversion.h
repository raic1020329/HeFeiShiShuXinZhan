//LatLong- UTM conversion..h
//definitions for lat/long to UTM and UTM to lat/lng conversions

#pragma once

#ifndef LATLONGCONV
#define LATLONGCONV


//墨卡托带状投影算法的坐标映射（0-8度为一个单元）
//这种方法有跨单元计算失败的风险。
//也就是一个区域如果刚好落再2个单元上，一个计算正确，另一个计算失败。
void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long, 
			 double &UTMNorthing, double &UTMEasting, char* UTMZone);
void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone,
			  double& Lat,  double& Long );
char UTMLetterDesignator(double Lat);

class Ellipsoid
{
public:
	Ellipsoid(){};
	Ellipsoid(int Id, char* name, double radius, double ecc)
	{
		id = Id; ellipsoidName = name; 
		EquatorialRadius = radius; eccentricitySquared = ecc;
	}

	int id;
	char* ellipsoidName;
	double EquatorialRadius;
	double eccentricitySquared;

};



//以任意点为中心的墨卡托带状投影算法的坐标映射
//这种方法的坐标转换适用于城市区域的GIS
//注意像北京到上海这种大面积的gis，下面的gis坐标映射误差很大。需要另外的方法来解决。
void LLtoTMERC(const double lonOrigin, const double latOrigin, double& x, double& y);
void TMERCtoLL(const double lonOrigin, const double latOrigin, double& x, double& y);




#endif

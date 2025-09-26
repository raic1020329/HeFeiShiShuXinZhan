// Copyright Epic Games, Inc. All Rights Reserved.

#include "CalculatedPolygonBPLibrary.h"
#include "CalculatedPolygon.h"
#include <Engine/Polys.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

UCalculatedPolygonBPLibrary::UCalculatedPolygonBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UCalculatedPolygonBPLibrary::GetWorldPosByMouse(UObject* WorldContextObject, FVector& OutPos)
{
	FVector start;
	FVector dir;
	FVector end;
	if (UGameplayStatics::GetPlayerController(WorldContextObject,0)->DeprojectMousePositionToWorld(start,dir))
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
		UE_LOG(LogTemp, Warning, TEXT("%s LineTraceSingle is not hit something"),*FString(__FUNCTION__));
		return returnValue;
	}

}

bool UCalculatedPolygonBPLibrary::IsPointInPolygon(const FVector2D& TestPoint, const TArray<FVector2D>& PolygonPoints)
{
	const int NumPoints = PolygonPoints.Num();
	float AngleSum = 0.0f;
	for (int PointIndex = 0; PointIndex < NumPoints; ++PointIndex)
	{
		const FVector2D& VecAB = PolygonPoints[PointIndex] - TestPoint;
		const FVector2D& VecAC = PolygonPoints[(PointIndex + 1) % NumPoints] - TestPoint;
		const float Angle = FMath::Sign(FVector2D::CrossProduct(VecAB, VecAC)) * FMath::Acos(FMath::Clamp(FVector2D::DotProduct(VecAB, VecAC) / (VecAB.Size() * VecAC.Size()), -1.0f, 1.0f));
		AngleSum += Angle;
	}
	return (FMath::Abs(AngleSum) > 0.001f);
}

float UCalculatedPolygonBPLibrary::MeasurePolyArea(const TArray<FVector3f>& Locs)
{
	FPoly NewPoly;
	NewPoly.Init();
	for (int i = 0; i < Locs.Num(); i++) {
		NewPoly.Vertices.Add(Locs[i]);
	}
	return NewPoly.Area();
}

bool UCalculatedPolygonBPLibrary::GetCenterOfVector2D(const TArray<FVector2D>& Points, FVector2D& Center)
{
	if (Points.Num() == 1)
	{
		Center = Points[0];
		return true;
	}
	else if (Points.Num() >= 2)
	{
		float MinX = Points[0].X, MaxX = Points[0].X;
		float MinY = Points[0].Y, MaxY = Points[0].Y;
		for (const FVector2D& Point : Points)
		{
			MinX = Point.X < MinX ? Point.X : MinX;
			MaxX = Point.X > MaxX ? Point.X : MaxX;
			MinY = Point.Y < MinY ? Point.Y : MinY;
			MaxY = Point.Y > MaxY ? Point.Y : MaxY;
		}
		Center = 0.5 * FVector2D(MaxX + MinX, MaxY + MinY);
		return true;
	}
	return false;
}

bool UCalculatedPolygonBPLibrary::GetCenterOfVector(const TArray<FVector>& Points, FVector& Center)
{
	if (Points.Num() == 1)
	{
		Center = Points[0];
		return true;
	}
	else if (Points.Num() >= 2)
	{
		float MinX = Points[0].X, MaxX = Points[0].X;
		float MinY = Points[0].Y, MaxY = Points[0].Y;
		float MinZ = Points[0].Z, MaxZ = Points[0].Z;
		for (const FVector& Point : Points)
		{
			MinX = Point.X < MinX ? Point.X : MinX;
			MaxX = Point.X > MaxX ? Point.X : MaxX;
			MinY = Point.Y < MinY ? Point.Y : MinY;
			MaxY = Point.Y > MaxY ? Point.Y : MaxY;
			MinZ = Point.Z < MinZ ? Point.Z : MinZ;
			MaxZ = Point.Z > MaxZ ? Point.Z : MaxZ;
		}
		Center = 0.5 * FVector(MaxX + MinX, MaxY + MinY, MaxZ + MinZ);
		return true;
	}
	return false;
}

float UCalculatedPolygonBPLibrary::CalculatePolygonArea(TArray<FVector> inputPoints)
{
	int point_num = inputPoints.Num();
	if (point_num < 3) return 0;
	double s = 0;
	for (int i = 0; i < point_num; ++i)
	{
		s += inputPoints[i].X * inputPoints[(i + 1) % point_num].Y - inputPoints[i].Y * inputPoints[(i + 1) % point_num].X;
	}

	return FMath::Abs(s * 0.5);
}

float UCalculatedPolygonBPLibrary::CalculateHorLength(const FVector& StartPoint, const FVector& EndPoint)
{
	return UKismetMathLibrary::Vector_Distance2D(StartPoint, EndPoint);
}

float UCalculatedPolygonBPLibrary::CalculateVerLength(const FVector& StartPoint, const FVector& EndPoint)
{
	return FMath::Abs(StartPoint.Z - EndPoint.Z);
}

float UCalculatedPolygonBPLibrary::CalculateVectorLength(const FVector& StartPoint, const FVector& EndPoint)
{
	return UKismetMathLibrary::Vector_Distance(StartPoint, EndPoint);
}

float UCalculatedPolygonBPLibrary::CalculateVectorAngle(const FVector& StartPoint, const FVector& CenterPoint, const FVector& EndPoint)
{
	FVector vector1 = StartPoint - CenterPoint;
	vector1.Normalize(0.0001f);
	FVector vector2 = EndPoint - CenterPoint;
	vector2.Normalize(0.0001f);
	return UKismetMathLibrary::DegAcos(FVector::DotProduct(vector1, vector2));
}

FVector UCalculatedPolygonBPLibrary::CalculatePointBetweenTwoPoints(const FVector& StartPoint, const FVector& EndPoint, float Invalue)
{
	FVector Normal = UKismetMathLibrary::Normal((EndPoint - StartPoint));
	return StartPoint + Normal * (FVector::Distance(StartPoint, EndPoint) * Invalue);
}

FVector2D UCalculatedPolygonBPLibrary::CalculateCenterOfPolygon(const TArray<FVector2D>& InPointArray)
{
	FVector2D center;


	auto Area = [](const FVector2D& p0, const FVector2D& p1, const FVector2D& p2)
	{
		double area = 0;
		area = p0.X * p1.Y + p1.X * p2.Y + p2.X * p0.Y - p1.X * p0.Y - p2.X * p1.Y - p0.X * p2.Y;
		return area / 2;
	};

	FVector2D p0 = InPointArray[0];
	FVector2D p1 = InPointArray[1];
	FVector2D p2;
	double sumarea = 0, sumx = 0, sumy = 0;

	for (int i = 2; i < InPointArray.Num(); i++)
	{
		p2 = InPointArray[i];
		double area = Area(p0, p1, p2);//求三角形的面积
		sumarea += area;
		sumx += (p0.X + p1.X + p2.X) * area; //求∑cx[i] * s[i]和∑cy[i] * s[i]
		sumy += (p0.Y + p1.Y + p2.Y) * area;
		p1 = p2;//求总面积
	}
	center.X = sumx / sumarea / 3;
	center.Y = sumy / sumarea / 3;




	return center;
}


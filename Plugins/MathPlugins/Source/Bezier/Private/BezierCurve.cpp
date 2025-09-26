// Fill out your copyright notice in the Description page of Project Settings.


#include "BezierCurve.h"
#include <DrawDebugHelpers.h>

// Sets default values
ABezierCurve::ABezierCurve()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Count = 50;
}

// Called when the game starts or when spawned
void ABezierCurve::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABezierCurve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FVector> ABezierCurve::BezierCurve(const TArray<FVector>& src)
{
	//不存在1阶
	if (src.Num() < 1)
	{
		return	src;
	}
	//-----------------------------t的分布步数------------------------------------
	const float	step = 1.0f/ Count;
	//---------------------------------------------------------------------------
	//	-----------------------------返回值-----------------------------------------
	TArray<FVector>	res;
	//	----------------------------------------------------------------------------

	//	---------------------------2阶 一条直线------------------------------------
	if (src.Num() == 1)
	{
		for (float t = 0; t < 1; t += step)
		{
			res.Add(src[0]);
		}
		return	res;
	}
	//	--------------------------------------------------------------------------


	//-----------------------------------3阶 及以上-----------------------------------------

	//---------------------------src1是起点到最后一个点的上一个点-----------------------------
	TArray<FVector> src1;
	//-------------------------------------------------------------------------------------

	//---------------------------src2是从第二个点到最后一个点---------------------------------
	TArray<FVector> src2;
	//--------------------------------------------------------------------------------------

	//---------------------------对src1,src2赋值 --------------------------------------------
	for (int32 i = 0; i < src.Num() - 1; i++)
	{
		src1.Add(src[i]);
		UE_LOG(LogTemp, Warning, TEXT("==%s\t"), *src1[i].ToString());
	}
	for (int32 j = 0; j < src.Num(); j++)
	{
		src2.Add(src[j]);
		UE_LOG(LogTemp, Warning, TEXT("==%s\t"), *src2[j].ToString());
	}
	src2.RemoveAt(0);
	//---------------------------------------------------------------------------------------

	//----------------------------递归调用所有的点--------------------------------------------
	TArray<FVector>	pln1 = BezierCurve(src1);

	TArray<FVector>	pln2 = BezierCurve(src2);
	//---------------------------------------------------------------------------------------
	for (float t = 0; t < 1; t += step)
	{
		FVector	tmp;
		//-------------------------------------应用贝塞尔曲线算法------------------------------
		tmp = ((1.0 - t) * pln1[FMath::RoundToInt(1.0 / step * t)]) + (t * pln2[FMath::RoundToInt(1.0 / step * t)]);
		//--------------------最终得到的点就是贝塞尔曲线上的点的位置了-----------------------
		res.Add(tmp);
	}
	return	res;
}



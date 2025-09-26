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
	//������1��
	if (src.Num() < 1)
	{
		return	src;
	}
	//-----------------------------t�ķֲ�����------------------------------------
	const float	step = 1.0f/ Count;
	//---------------------------------------------------------------------------
	//	-----------------------------����ֵ-----------------------------------------
	TArray<FVector>	res;
	//	----------------------------------------------------------------------------

	//	---------------------------2�� һ��ֱ��------------------------------------
	if (src.Num() == 1)
	{
		for (float t = 0; t < 1; t += step)
		{
			res.Add(src[0]);
		}
		return	res;
	}
	//	--------------------------------------------------------------------------


	//-----------------------------------3�� ������-----------------------------------------

	//---------------------------src1����㵽���һ�������һ����-----------------------------
	TArray<FVector> src1;
	//-------------------------------------------------------------------------------------

	//---------------------------src2�Ǵӵڶ����㵽���һ����---------------------------------
	TArray<FVector> src2;
	//--------------------------------------------------------------------------------------

	//---------------------------��src1,src2��ֵ --------------------------------------------
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

	//----------------------------�ݹ�������еĵ�--------------------------------------------
	TArray<FVector>	pln1 = BezierCurve(src1);

	TArray<FVector>	pln2 = BezierCurve(src2);
	//---------------------------------------------------------------------------------------
	for (float t = 0; t < 1; t += step)
	{
		FVector	tmp;
		//-------------------------------------Ӧ�ñ����������㷨------------------------------
		tmp = ((1.0 - t) * pln1[FMath::RoundToInt(1.0 / step * t)]) + (t * pln2[FMath::RoundToInt(1.0 / step * t)]);
		//--------------------���յõ��ĵ���Ǳ����������ϵĵ��λ����-----------------------
		res.Add(tmp);
	}
	return	res;
}



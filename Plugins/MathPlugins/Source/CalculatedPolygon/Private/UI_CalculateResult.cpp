// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CalculateResult.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <CalculatedPolygonBPLibrary.h>
#include <Kismet/GameplayStatics.h>


void UUI_CalculateResult::NativePreConstruct()
{
	Super::NativePreConstruct();


}

void UUI_CalculateResult::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_CalculateResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
	

	switch (m_CalculateType)
	{
	case E_CalculateType::eNone: {
		LengthPoint();
		return;
	}
	case E_CalculateType::eHorizontal: {
		LengthPoint();
		return;
	}
	case E_CalculateType::eVertical: {
		LengthPoint();
		return;
	}
	case E_CalculateType::eVector: {
		LengthPoint();
		return;
	}
	case E_CalculateType::eAngle: {
		AnglePoint();
		return;
	}
	case E_CalculateType::eArea: {
		AreaPoint();
		return;
	}
	}
	


	


}

void UUI_CalculateResult::LengthPoint()
{
	UWorld* const MyWorld = GetWorld();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(MyWorld, 0);
	FVector2D FirstScreenPos;
	FVector2D EndScreenPos;
	if (m_WorldPosArray.IsValidIndex(1))
	{
		if (PlayerController->ProjectWorldLocationToScreen(m_WorldPosArray[0], FirstScreenPos) && PlayerController->ProjectWorldLocationToScreen(m_WorldPosArray[1], EndScreenPos))
		{
			FVector2D newScrenPos = FVector2D(UCalculatedPolygonBPLibrary::CalculatePointBetweenTwoPoints(FVector(FirstScreenPos, 0), FVector(EndScreenPos, 0), 0.5f));
			SetPositionInViewport(newScrenPos);
		}
	}
	
}

void UUI_CalculateResult::AnglePoint()
{
	UWorld* const MyWorld = GetWorld();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(MyWorld, 0);
	FVector2D newScrenPos;
	if (m_WorldPosArray.IsValidIndex(0))
	{
		if (PlayerController->ProjectWorldLocationToScreen(m_WorldPosArray[0], newScrenPos))
		{
			SetPositionInViewport(newScrenPos);
		}
	}
}

void UUI_CalculateResult::AreaPoint()
{
	UWorld* const MyWorld = GetWorld();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(MyWorld, 0);
	TArray<FVector2D> newScrenPosArray;
	for (const FVector& k :m_WorldPosArray)
	{
		FVector2D newScrenPos;
		if (PlayerController->ProjectWorldLocationToScreen(k, newScrenPos))
		{
			newScrenPosArray.Emplace(newScrenPos);
		}
	}
	SetPositionInViewport(UCalculatedPolygonBPLibrary::CalculateCenterOfPolygon(newScrenPosArray));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CalculatePolygon.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <CalculatedPolygonBPLibrary.h>
#include <UI_CalculateResult.h>
#include <Kismet/KismetTextLibrary.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/StaticMeshActor.h>

#define LOCTEXT_NAMESPACE "UUI_CalculatePolygon"

//FLinearColor UUI_CalculatePolygon::m_LineColor = FLinearColor(1, 1, 1);

UUI_CalculatePolygon::UUI_CalculatePolygon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), m_bLineStart(false),m_LineColor(FLinearColor(1, 1, 1, 1))
{
	m_ScreenPos.Empty();
	m_WorldPos.Empty();
	m_CalculateResult.Empty();
}

void UUI_CalculatePolygon::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UUI_CalculatePolygon::NativeConstruct()
{
	Super::NativeConstruct();
	//m_LineColor = FLinearColor(1, 0.630757f, 0, 0.8f);
	//m_LineColor = FLinearColor(1, 0.0569f, 0, 1);


}

void UUI_CalculatePolygon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (m_bLineStart)
	{
		CreateUI();
	}
}

int32 UUI_CalculatePolygon::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	for (int32 i = 0; i < m_WorldPos.Num(); ++i)
	{
		if (m_WorldPos.Num() >= 2) {
			if (i < m_WorldPos.Num() - 1)
			{
				UWorld* const MyWorld = GetWorld();
				FVector2D ScreenPosition1;
				FVector2D ScreenPosition2;
				APlayerController* playerColtroller = UGameplayStatics::GetPlayerController(MyWorld, 0);
				bool success1 = playerColtroller->ProjectWorldLocationToScreen(m_WorldPos[i], ScreenPosition1);
				bool success2 = playerColtroller->ProjectWorldLocationToScreen(m_WorldPos[i + 1], ScreenPosition2);
				if (success1 && success2)
				{
					const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(MyWorld);
					const float RelativeScale = FMath::Pow(ViewportScale, -1);
					ScreenPosition1 *= RelativeScale;
					ScreenPosition2 *= RelativeScale;
					//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, ScreenPosition1.ToString());
					//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, ScreenPosition2.ToString());
					//auto InClass = GetClass();

					//auto CDO = InClass->ClassDefaultObject;
					UWidgetBlueprintLibrary::DrawLine(Context, ScreenPosition1, ScreenPosition2, m_LineColor/*Cast<UUI_CalculatePolygon>(CDO)->m_LineColor*/);
				}
				//FString InText = GetText(m_CalculateType, m_WorldPos[i], m_WorldPos[i + 1]);
				//FVector center = UCalculatedPolygonBPLibrary::CalculatePointBetweenTwoPoints(FVector(m_ScreenPos[i], 0), FVector(m_ScreenPos[i + 1], 0), 0.5f);
				//UWidgetBlueprintLibrary::DrawText(Context, InText, FVector2D(center), FLinearColor::Red);
				//CreateResult(FVector2D(center), InText);

			}
			else
			{
				UWorld* const MyWorld = GetWorld();
				FVector2D ScreenPosition1;
				FVector2D ScreenPosition2;
				APlayerController* playerColtroller = UGameplayStatics::GetPlayerController(MyWorld, 0);
				bool success1 = playerColtroller->ProjectWorldLocationToScreen(m_WorldPos[i], ScreenPosition1);
				bool success2 = playerColtroller->ProjectWorldLocationToScreen(m_WorldPos[0], ScreenPosition2);
				if (success1 && success2)
				{
					const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(MyWorld);
					const float RelativeScale = FMath::Pow(ViewportScale, -1);
					ScreenPosition1 *= RelativeScale;
					ScreenPosition2 *= RelativeScale;
					UWidgetBlueprintLibrary::DrawLine(Context, ScreenPosition1, ScreenPosition2, m_LineColor);
				}
				//FString InText = GetText(m_CalculateType, m_WorldPos[i], m_WorldPos[0]);
				//FVector center = UCalculatedPolygonBPLibrary::CalculatePointBetweenTwoPoints(FVector(m_ScreenPos[i], 0), FVector(m_ScreenPos[0], 0), 0.5f);
				//UWidgetBlueprintLibrary::DrawText(Context, InText, FVector2D(center), FLinearColor::Red);
				//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, center.ToString());
				//CreateResult(FVector2D(center), InText);
			}
		}
	}


	return LayerId;
}


FString UUI_CalculatePolygon::GetText(const E_CalculateType& InType, const FVector& InStartPos, const FVector& InEndPos) const
{
	switch (InType)
	{
	case E_CalculateType::eNone: {
		const float cm = UCalculatedPolygonBPLibrary::CalculateVectorLength(InStartPos, InEndPos);
		return UKismetTextLibrary::Conv_DoubleToText(cm * 0.01, ERoundingMode::HalfToEven, false, false, 1, 324, 0, 2).ToString() + FString(TEXT("m"));
	}
	case E_CalculateType::eHorizontal: {
		const float cm = UCalculatedPolygonBPLibrary::CalculateHorLength(InStartPos, InEndPos);
		return UKismetTextLibrary::Conv_DoubleToText(cm * 0.01, ERoundingMode::HalfToEven, false, false, 1, 324, 0, 2).ToString() + FString(TEXT("m"));
	}
	case E_CalculateType::eVertical: {
		const float cm = UCalculatedPolygonBPLibrary::CalculateVerLength(InStartPos, InEndPos);
		return UKismetTextLibrary::Conv_DoubleToText(cm * 0.01, ERoundingMode::HalfToEven, false, false, 1, 324, 0, 2).ToString() + FString(TEXT("m"));
	}
	case E_CalculateType::eVector: {
		const float cm = UCalculatedPolygonBPLibrary::CalculateVectorLength(InStartPos, InEndPos);
		return UKismetTextLibrary::Conv_DoubleToText(cm * 0.01, ERoundingMode::HalfToEven, false, false, 1, 324, 0, 2).ToString() + FString(TEXT("m"));
	}
	case E_CalculateType::eAngle: {
		return FString(TEXT("look message"));
	}
	case E_CalculateType::eArea: {
		return FString(TEXT("look message"));
	}
	}
	return FString(TEXT("No Search Any Message"));
}

void UUI_CalculatePolygon::ClearData()
{
	m_ScreenPos.Empty();
	m_WorldPos.Empty();
	m_CalculateType = E_CalculateType::eNone;
}

void UUI_CalculatePolygon::CreateUI()
{
	CreateResult();
}

void UUI_CalculatePolygon::CreateLength()
{

	if (m_CalculateResult.Num() < m_ScreenPos.Num() && m_ScreenPos.Num() >= 2)
	{
		if (CurrentCount < m_ScreenPos.Num())
		{
			for (UUI_CalculateResult* var : m_CalculateResult)
			{
				if (var->IsValidLowLevel())
					var->RemoveFromParent();
			}
			m_CalculateResult.Empty();
			CurrentCount = 0;
		}
		for (int32 i = 0; i < m_ScreenPos.Num(); i++)
		{
			if (m_ScreenPos.Num() >= 2)
			{
				UUI_CalculateResult* tempResult = CreateWidget<UUI_CalculateResult>(GetWorld(), MyWidget);
				if (tempResult)
				{
					tempResult->AddToViewport(0);
					if (i < m_ScreenPos.Num() - 1)
					{
						FString InText = GetText(m_CalculateType, m_WorldPos[i], m_WorldPos[i + 1]);
						FVector center = UCalculatedPolygonBPLibrary::CalculatePointBetweenTwoPoints(FVector(m_ScreenPos[i], 0), FVector(m_ScreenPos[i + 1], 0), 0.5f);
						//const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
						//center /= Scale;
						TArray<FVector> newposs;
						newposs.Emplace(m_WorldPos[i]);
						newposs.Emplace(m_WorldPos[i + 1]);
						tempResult->SetValue(newposs);
						tempResult->SetValue(FText::FromString(InText));
						tempResult->SetPositionInViewport(FVector2D(center), false);
						//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, center.ToString());
						m_CalculateResult.Emplace(tempResult);
						CurrentCount++;
					}
					else
					{
						FString InText = GetText(m_CalculateType, m_WorldPos[i], m_WorldPos[0]);
						FVector center = UCalculatedPolygonBPLibrary::CalculatePointBetweenTwoPoints(FVector(m_ScreenPos[i], 0), FVector(m_ScreenPos[0], 0), 0.5f);
						//const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
						//center /= Scale;
						TArray<FVector> newposs;
						newposs.Emplace(m_WorldPos[i]);
						newposs.Emplace(m_WorldPos[0]);
						tempResult->SetValue(newposs);
						tempResult->SetValue(FText::FromString(InText));
						tempResult->SetPositionInViewport(FVector2D(center), false);
						//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, center.ToString());
						m_CalculateResult.Emplace(tempResult);
						CurrentCount++;
					}
				}
			}

		}

	}
}

void UUI_CalculatePolygon::CreateAngel()
{

	if (m_ScreenPos.Num() > 2 && m_ScreenPos.Num() > m_CalculateResult.Num() + 2)
	{
		if (CurrentCount < m_ScreenPos.Num() - 2)
		{
			for (UUI_CalculateResult* var : m_CalculateResult)
			{
				if (var->IsValidLowLevel())
					var->RemoveFromParent();
			}
			m_CalculateResult.Empty();
			CurrentCount = 0;
		}
		int32 WorldPosCount = m_WorldPos.Num();
		for (int32 i = 1; i < WorldPosCount - 1; i++)
		{
			UUI_CalculateResult* tempResult = CreateWidget<UUI_CalculateResult>(GetWorld(), MyWidget);
			if (tempResult)
			{
				tempResult->AddToViewport(0);
				float angle = 0;
				if (m_WorldPos.IsValidIndex(i - 1) && m_WorldPos.IsValidIndex(i) && m_WorldPos.IsValidIndex(i + 1))
				{
					angle = UCalculatedPolygonBPLibrary::CalculateVectorAngle(m_WorldPos[i - 1], m_WorldPos[i], m_WorldPos[i + 1]);
				}
				TArray<FVector> newPos;
				newPos.Emplace(m_WorldPos[i]);
				tempResult->SetValue(newPos, E_CalculateType::eAngle);
				tempResult->SetValue(FText::Format(LOCTEXT("ExampleFText", "{0}°"), UKismetTextLibrary::Conv_DoubleToText(angle, ERoundingMode::HalfToEven, false, false, 1, 324, 0, 1)));
				tempResult->SetPositionInViewport(m_ScreenPos[i], false);
				m_CalculateResult.Emplace(tempResult);
				CurrentCount++;
			}
		}
	}
}

void UUI_CalculatePolygon::CreateArea()
{
	if (m_ScreenPos.Num() >= 3 && m_ResetArea)
	{
		if (CurrentCount == 1)
		{
			for (UUI_CalculateResult* var : m_CalculateResult)
			{
				if (var->IsValidLowLevel())
					var->RemoveFromParent();
			}
			m_CalculateResult.Empty();
			CurrentCount = 0;
		}
		UUI_CalculateResult* tempResult = CreateWidget<UUI_CalculateResult>(GetWorld(), MyWidget);
		if (tempResult)
		{
			tempResult->AddToViewport(0);
			float area = 0;
			area = UCalculatedPolygonBPLibrary::CalculatePolygonArea(m_WorldPos);
			tempResult->SetValue(m_WorldPos, E_CalculateType::eArea);
			tempResult->SetValue(FText::Format(LOCTEXT("ExampleFText", "{0}m²"), UKismetTextLibrary::Conv_DoubleToText(area * 0.0001, ERoundingMode::HalfToEven, false, false, 1, 324, 0, 2)));
			tempResult->SetPositionInViewport(UCalculatedPolygonBPLibrary::CalculateCenterOfPolygon(m_ScreenPos), false);
			m_CalculateResult.Emplace(tempResult);
			CurrentCount++;
			m_ResetArea = false;
		}

	}
}

void UUI_CalculatePolygon::OpenMeshCollision()
{
	//TFunction<void()> LamamFun = [=]()
	//{
	//	if (!MeshIsOpenCollision)
	//	{
	//		//不能在游戏线程以外的线程操作object
	//		//TArray<AActor*> OutMesh;
	//		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), OutMesh);
	//		//for (AActor* actor : OutMesh)
	//		//{
	//		//	AStaticMeshActor* mesh = Cast<AStaticMeshActor>(actor);
	//		//	//如果模型没隐藏 并且不能被鼠标点击的模型  打开visibility
	//		//	if (!mesh->IsHidden() && !(mesh->GetStaticMeshComponent()->GetCollisionResponseToChannel(ECC_Visibility) == ECollisionResponse::ECR_Block))
	//		//	{
	//		//		OpenCollisionMeshArray.Emplace(mesh);
	//		//		mesh->SetActorEnableCollision(true);
	//		//		mesh->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//		//		mesh->GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//		//		mesh->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	//		//	}
	//		//}
	//		MeshIsOpenCollision = true;
	//	}
	//};
	//auto Task = new FAutoDeleteAsyncTask<FOpenCollisionAnsyTask>(LamamFun);
	//Task->StartBackgroundTask();
}

void UUI_CalculatePolygon::CloseMeshCollision()
{
	//TFunction<void()> LamamFun = [=]()
	//{
	//	//不能在游戏线程以外的线程操作object类
	//	/*if (MeshIsOpenCollision)
	//	{
	//		for (AStaticMeshActor* mesh : OpenCollisionMeshArray)
	//		{
	//			mesh->GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//			mesh->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//			mesh->SetActorEnableCollision(false);
	//		}
	//		MeshIsOpenCollision = false;
	//	}*/
	//};
	//auto Task = new FAutoDeleteAsyncTask<FOpenCollisionAnsyTask>(LamamFun);
	//Task->StartBackgroundTask();
}

void UUI_CalculatePolygon::CreateResult_Implementation()
{
	switch (m_CalculateType)
	{
	case E_CalculateType::eNone: {
		CreateLength();
		return;
	}
	case E_CalculateType::eHorizontal: {
		CreateLength();
		return;
	}
	case E_CalculateType::eVertical: {
		CreateLength();
		return;
	}
	case E_CalculateType::eVector: {
		CreateLength();
		return;
	}
	case E_CalculateType::eAngle: {
		CreateAngel();
		return;
	}
	case E_CalculateType::eArea: {
		CreateArea();
		return;
	}
	default:
		break;
	}
}


#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BezierCurve.generated.h"

UCLASS()
class BEZIER_API ABezierCurve : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABezierCurve();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable ,Category = "BezierCurve")
	TArray<FVector> BezierCurve(const TArray<FVector>& src );

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Count;
};

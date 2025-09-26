// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CameraAnchorPoint.h"
#include "DTCameraSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DTFRAMETEMPLETE_API UDTCameraSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FCameraSetting> CameraSettingMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FCameraLimit> CameraLimitMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap < FName, ECameraType> CameraTypeMap;
};

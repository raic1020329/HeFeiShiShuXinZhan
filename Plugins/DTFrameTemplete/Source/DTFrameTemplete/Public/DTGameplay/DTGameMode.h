// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DTGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DTFRAMETEMPLETE_API ADTGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ADTGameMode();
	void BeginPlay() override;
};

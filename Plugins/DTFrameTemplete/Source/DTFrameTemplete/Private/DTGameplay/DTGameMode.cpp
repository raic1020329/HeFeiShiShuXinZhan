// Fill out your copyright notice in the Description page of Project Settings.

#include "DTGameplay/DTGameMode.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DTGameplay/DTPawn.h"
#include "DTGameplay/DTHUD.h"
#include "DTGameplay/DTPlayerController.h"
ADTGameMode::ADTGameMode()
{
	DefaultPawnClass = ADTPawn::StaticClass();
	HUDClass = ADTHUD::StaticClass();
	PlayerControllerClass = ADTPlayerController::StaticClass();

}

void ADTGameMode::BeginPlay()
{
	Super::BeginPlay();
}

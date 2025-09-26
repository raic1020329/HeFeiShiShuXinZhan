// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeFeiShiShuXinZhanGameMode.h"
#include "HeFeiShiShuXinZhanCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHeFeiShiShuXinZhanGameMode::AHeFeiShiShuXinZhanGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

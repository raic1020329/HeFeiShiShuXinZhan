// Fill out your copyright notice in the Description page of Project Settings.


#include "DTGameplay/DTHUD.h"

void ADTHUD::BeginPlay()
{
	Super::BeginPlay();
	if (!MainUI && MainUIClass)
	{
		MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
		MainUI->AddToViewport(0);
	}
}

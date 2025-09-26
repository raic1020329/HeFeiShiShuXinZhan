// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainUI.h"
#include "DTHUD.generated.h"

/**
 * 
 */
UCLASS()
class DTFRAMETEMPLETE_API ADTHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DTHUD")
	TSubclassOf<UMainUI> MainUIClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DTHUD")
	UMainUI* MainUI;
};

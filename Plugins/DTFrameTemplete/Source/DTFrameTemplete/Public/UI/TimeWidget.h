// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeWidget.generated.h"

/**
 * 
 */
class UCanvasPanel;
class UTextBlock;

UCLASS(BlueprintType, Blueprintable)
class DTFRAMETEMPLETE_API UTimeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick (const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	//各个UI的模块存放的容器
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "TimeUI")
	UCanvasPanel* CanvasPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "TimeUI")
	UTextBlock* TextBlock_Date;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "TimeUI")
	UTextBlock* TextBlock_Time;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "TimeUI")
	UTextBlock* TextBlock_Week;

	UFUNCTION()
	void GetTime();

private:
	FTimerHandle TimeHandle;

	UPROPERTY(EditDefaultsOnly, Category = "TimeUI")
	float TimerChecked = 0.1;
};

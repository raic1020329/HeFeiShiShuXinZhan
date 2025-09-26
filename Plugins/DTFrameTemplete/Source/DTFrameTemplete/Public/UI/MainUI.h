// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UUIBase;
class UCanvasPanel;

USTRUCT(BlueprintType)
struct FModulUIInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularUIInfo")
	FName CameraTag = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularUIInfo")
	TSubclassOf<UUIBase> UIBaseClass = nullptr;
};
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DTFRAMETEMPLETE_API UMainUI : public UUserWidget 
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
public:
	//各个UI的模块存放的容器
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainUI")
	UCanvasPanel* RootCanvasPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainUI")
	UCanvasPanel* UIContainerPanel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBase")
	FName ModuleTag;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainUI")
	TMap<FName, TSubclassOf<UUIBase>> UIBaseClassMap; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainUI")
	UUIBase* CurrentUIModule;
public:
	UFUNCTION(BlueprintCallable,Category="MainUI")
	void ChangeModule(FName InModuleTag);
};

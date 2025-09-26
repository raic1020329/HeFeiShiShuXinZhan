// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DTCameraSaveGame.h"
#include "DTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DTFRAMETEMPLETE_API UDTGameInstance : public UGameInstance
{
	GENERATED_BODY()
		UUserWidget* LoadingWidget;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FDateTime m_ExpiredTime = FDateTime(2025,7,22);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGGame")
		TSubclassOf<UUserWidget> LoadingWidgetClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGGame")
		bool bAutoCompleteWhenLoadingCompletes = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGGame")
		bool bWaitForManualStop = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGGame")
		bool bMoviesAreSkippable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGGame")
		float MinimumLoadingScreenDisplayTime = 3.0f;

	virtual void Init() override;

	virtual void Shutdown() override;


	UFUNCTION(BlueprintCallable, Category = "CameraSave")
	void SaveCameraSaveGame(FName CameraTag, FCameraSetting Setting, FCameraLimit CameraLimit, ECameraType InType);


protected:
	void InitDTCameraSaveGame();
private:
	UPROPERTY()
	UDTCameraSaveGame* DTCameraSaveGame;

	bool CheckExpiration();
	void ShowExpirationMessage();

private:
	//void Init();
	void BeginLoading(const FString& Map);
	void EndLoading(const FWorldContext& World, const FString& Map);
};

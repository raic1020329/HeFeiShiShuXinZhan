// Fill out your copyright notice in the Description page of Project Settings.


#include "DTGameplay/DTGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

void UDTGameInstance::Init()
{
	if (!CheckExpiration())
	{
		ShowExpirationMessage();
		FPlatformMisc::RequestExit(true); // 退出程序
	}
	Super::Init();
	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDTGameInstance::BeginLoading);
	//FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &UDTGameInstance::EndLoading);
}

void UDTGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UDTGameInstance::InitDTCameraSaveGame()
{
	if (!DTCameraSaveGame)
	{
		DTCameraSaveGame = NewObject<UDTCameraSaveGame>(GetTransientPackage(), UDTCameraSaveGame::StaticClass());
	}
}

bool UDTGameInstance::CheckExpiration()
{
	FDateTime CurrentTime = FDateTime::Now();
	FDateTime ExpirationTime = m_ExpiredTime; // 设置过期时间

	return CurrentTime < ExpirationTime;
}
void UDTGameInstance::ShowExpirationMessage()
{
	// 使用UE的消息对话框
	FText Title = FText::FromString("License Expired");
	FText Message = FText::FromString("This version has expired. Please contact developer for renewal.");
	FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
}


void UDTGameInstance::BeginLoading(const FString& Map)
{
	/*FLoadingScreenAttributes LoadingScreen;

	LoadingScreen.bAutoCompleteWhenLoadingCompletes = bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.MinimumLoadingScreenDisplayTime = MinimumLoadingScreenDisplayTime;
	LoadingScreen.bWaitForManualStop = bWaitForManualStop;
	LoadingScreen.bMoviesAreSkippable = bMoviesAreSkippable;

	if (LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		if (!LoadingWidget)
		{
			LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		}
		TSharedPtr<SWidget>WidgetPtr = LoadingWidget->TakeWidget();
		LoadingScreen.WidgetLoadingScreen = WidgetPtr;
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}*/
	
	//LoadingScreen.MinimumLoadingScreenDisplayTime = EMoviePlaybackType::MT_Looped;
}

void UDTGameInstance::EndLoading(const FWorldContext& World, const FString& Map)
{
	//GetMoviePlayer()->StopMovie();
}

void UDTGameInstance::SaveCameraSaveGame(FName CameraTag, FCameraSetting Setting, FCameraLimit CameraLimit, ECameraType InType)
{
	InitDTCameraSaveGame();
	if (DTCameraSaveGame)
	{
		if (!DTCameraSaveGame->CameraSettingMap.Contains(CameraTag))
		{
			DTCameraSaveGame->CameraSettingMap.Add(CameraTag, Setting);
		}
		else
		{
			DTCameraSaveGame->CameraSettingMap[CameraTag]= Setting;
		}
		if (!DTCameraSaveGame->CameraLimitMap.Contains(CameraTag))
		{
			DTCameraSaveGame->CameraLimitMap.Add(CameraTag, CameraLimit);
		}
		else
		{
			DTCameraSaveGame->CameraLimitMap[CameraTag] = CameraLimit;
		}
		if (!DTCameraSaveGame->CameraTypeMap.Contains(CameraTag))
		{
			DTCameraSaveGame->CameraTypeMap.Add(CameraTag, InType);
		}
		else
		{
			DTCameraSaveGame->CameraTypeMap[CameraTag] = InType;
		}
		UGameplayStatics::SaveGameToSlot(DTCameraSaveGame, "CameraSaveGame", 0);
	}
}

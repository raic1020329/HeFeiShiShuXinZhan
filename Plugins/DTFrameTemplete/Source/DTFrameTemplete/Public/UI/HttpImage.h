// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Image.h"
#include "Http.h"
#include "GenericPlatform/GenericPlatformFile.h"

#include "HttpImage.generated.h"


class UTexture2DDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImageReceivedDelegate, UTexture2DDynamic*, Texture);

/**
 * 
 */
UCLASS()
class UHttpImage : public UImage
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "HttpImage")
		void SetHttpRequest(FString URL, bool bNeedCover = false, bool bIsStatic = false, bool bInAutoSetImage = true ,bool bInMatchSize = false);

	UFUNCTION(BlueprintCallable, Category = "HttpImage")
		void SetHttpRequestWithToken(FString URL, FString TokenKey, bool bNeedCover = false, bool bIsStatic = false, bool bInAutoSetImage = true);

	UFUNCTION(BlueprintCallable, Category = "HttpImage")
		void SetHttpRequest_ForSuperAPI(FString URL, bool bNeedCover = false, bool bIsStatic = false, bool bInAutoSetImage = true);

	UPROPERTY(BlueprintAssignable)
		FImageReceivedDelegate OnImageReceived;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 m_Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 m_Height;
	
private:
	FString ImageFileName;
	FString ImageUrl;
	FString CurrentURL;
	bool bAutoSetImage;
	bool bHaveToken;
	FString TokenString;
	bool bMatchSize;

	void SendRequest(const FString url);
	void OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	UTexture2DDynamic* LoadImageToTextureByPath(const FString ImageFilePath);
	UTexture2DDynamic* DataToTexture(const TArray<uint8> Data);
	
};


class FImageDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		if (bIsDirectory)
		{
			Directories.Add(FilenameOrDirectory);
		}
		else
		{
			Files.Add(FilenameOrDirectory);
		}
		return true;
	}

	TArray<FString> Directories;
	TArray<FString> Files;
};
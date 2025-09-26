#include "../../Public/UI/HttpImage.h"
#include "HAL/PlatformFilemanager.h"
#include "Logging/LogMacros.h"
#include "Misc/FileHelper.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Engine/Texture2DDynamic.h"
#include "RenderingThread.h"
#include "Modules/ModuleManager.h"
#include "RHICommandList.h"
#include "CoreGlobals.h"

#define LOCTEXT_NAMESPACE "UHttpImage"

#define DYNAMIC_PATH "DynamicImage"
#define STATIC_PATH "StaticImage"

UHttpImage::UHttpImage(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void UHttpImage::SetHttpRequest(FString URL, bool bNeedCover, bool bIsStatic /*= false*/, bool bInAutoSetImage, bool bInMatchSize)
{
	this->bAutoSetImage = bInAutoSetImage;
	if (!bNeedCover && URL == CurrentURL) return;
	this->bMatchSize = bInMatchSize;

	CurrentURL = URL;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FDateTime DataTime = FDateTime::UtcNow();

	FString ImageFolderPath = FPaths::ProjectSavedDir() + DYNAMIC_PATH;
	if (!PlatformFile.DirectoryExists(*ImageFolderPath))
		PlatformFile.CreateDirectory(*ImageFolderPath);

	FString NewImageFolderPath = ImageFolderPath + "/" + FString::Printf(TEXT("%d_%d_%d"), DataTime.GetYear(), DataTime.GetMonth(), DataTime.GetDay());
	if (!PlatformFile.DirectoryExists(*NewImageFolderPath))
		PlatformFile.CreateDirectory(*NewImageFolderPath);


	ImageFileName = NewImageFolderPath + "/" + FPaths::GetCleanFilename(URL);
	if (bIsStatic)
	{
		ImageFolderPath = FPaths::ProjectSavedDir() + STATIC_PATH;
		ImageFileName = ImageFolderPath + "/" + FPaths::GetCleanFilename(URL);
		if (!PlatformFile.DirectoryExists(*ImageFolderPath))
			PlatformFile.CreateDirectory(*ImageFolderPath);
	}

	if (bNeedCover && PlatformFile.FileExists(*ImageFileName))
	{
		PlatformFile.DeleteFile(*ImageFileName);
	}

	//查找ImageFolderPath下图片 如果找到就加载图片，未找到就下载 
	FImageDirectoryVisitor TemplateDirectoryVisitor;
	PlatformFile.IterateDirectoryRecursively(*ImageFolderPath, TemplateDirectoryVisitor);

	if (!bIsStatic)//同时删除DynamicImage下多余日期文件夹
	{
		for (FString Directory : TemplateDirectoryVisitor.Directories)
		{
			if (FPaths::ConvertRelativePathToFull(Directory) != FPaths::ConvertRelativePathToFull(NewImageFolderPath))
			{
				if (!IFileManager::Get().DeleteDirectory(*FPaths::ConvertRelativePathToFull(Directory), true, true))
				{
					UE_LOG(LogTemp, Error, TEXT("DeleteDirectory Faild ,Path : %s"), *FPaths::ConvertRelativePathToFull(Directory));
				}
			}
		}
	}
	if (TemplateDirectoryVisitor.Files.Contains(ImageFileName))
	{
		//直接加载图片
		auto Texture = LoadImageToTextureByPath(ImageFileName);
		if (!Texture)
		{

			return;

		}

		m_Width = Texture->GetSurfaceWidth();
		m_Height = Texture->GetSurfaceHeight();
		if (bAutoSetImage)
		{
			SetBrushFromTextureDynamic(Texture, bMatchSize);

		}
		
	}
	else
	{
		SendRequest(URL);
	}
}



void UHttpImage::SetHttpRequestWithToken(FString URL, FString TokenKey, bool bNeedCover, bool bIsStatic /*= false*/, bool bInAutoSetImage)
{

	if (!TokenKey.IsEmpty() && TokenKey != TEXT(""))
	{
		bHaveToken = true;
		TokenString = TokenKey;
	}
	else
	{
		bHaveToken = false;
	}


	this->bAutoSetImage = bInAutoSetImage;
	if (!bNeedCover && URL == CurrentURL) return;

	CurrentURL = URL;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FDateTime DataTime = FDateTime::UtcNow();

	FString ImageFolderPath = FPaths::ProjectSavedDir() + DYNAMIC_PATH;
	if (!PlatformFile.DirectoryExists(*ImageFolderPath))
		PlatformFile.CreateDirectory(*ImageFolderPath);

	FString NewImageFolderPath = ImageFolderPath + "/" + FString::Printf(TEXT("%d_%d_%d"), DataTime.GetYear(), DataTime.GetMonth(), DataTime.GetDay());
	if (!PlatformFile.DirectoryExists(*NewImageFolderPath))
		PlatformFile.CreateDirectory(*NewImageFolderPath);


	ImageFileName = NewImageFolderPath + "/" + FPaths::GetCleanFilename(URL);
	if (bIsStatic)
	{
		ImageFolderPath = FPaths::ProjectSavedDir() + STATIC_PATH;
		ImageFileName = ImageFolderPath + "/" + FPaths::GetCleanFilename(URL);
		if (!PlatformFile.DirectoryExists(*ImageFolderPath))
			PlatformFile.CreateDirectory(*ImageFolderPath);
	}

	if (bNeedCover && PlatformFile.FileExists(*ImageFileName))
	{
		PlatformFile.DeleteFile(*ImageFileName);
	}

	//查找ImageFolderPath下图片 如果找到就加载图片，未找到就下载 
	FImageDirectoryVisitor TemplateDirectoryVisitor;
	PlatformFile.IterateDirectoryRecursively(*ImageFolderPath, TemplateDirectoryVisitor);

	if (!bIsStatic)//同时删除DynamicImage下多余日期文件夹
	{
		for (FString Directory : TemplateDirectoryVisitor.Directories)
		{
			if (FPaths::ConvertRelativePathToFull(Directory) != FPaths::ConvertRelativePathToFull(NewImageFolderPath))
			{
				if (!IFileManager::Get().DeleteDirectory(*FPaths::ConvertRelativePathToFull(Directory), true, true))
				{
					UE_LOG(LogTemp, Error, TEXT("DeleteDirectory Faild ,Path : %s"), *FPaths::ConvertRelativePathToFull(Directory));
				}
			}
		}
	}
	if (TemplateDirectoryVisitor.Files.Contains(ImageFileName))
	{
		//直接加载图片
		auto Texture = LoadImageToTextureByPath(ImageFileName);
		if (!Texture)
		{

			return;

		}

		m_Width = Texture->GetSurfaceWidth();
		m_Height = Texture->GetSurfaceHeight();
		if (bAutoSetImage)
		{
			SetBrushFromTextureDynamic(Texture, false);
		}
	}
	else
	{
		SendRequest(URL);
	}
}

void UHttpImage::SetHttpRequest_ForSuperAPI(FString URL, bool bNeedCover, bool bIsStatic, bool bInAutoSetImage)
{
	this->bAutoSetImage = bInAutoSetImage;
	if (!bNeedCover && URL == CurrentURL) return;

	CurrentURL = URL;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FDateTime DataTime = FDateTime::UtcNow();

	FString ImageFolderPath = FPaths::ProjectDir() + DYNAMIC_PATH;
	if (!PlatformFile.DirectoryExists(*ImageFolderPath))
		PlatformFile.CreateDirectory(*ImageFolderPath);

	FString NewImageFolderPath = ImageFolderPath + "/" + FString::Printf(TEXT("%d_%d_%d"), DataTime.GetYear(), DataTime.GetMonth(), DataTime.GetDay());
	if (!PlatformFile.DirectoryExists(*NewImageFolderPath))
		PlatformFile.CreateDirectory(*NewImageFolderPath);


	ImageFileName = NewImageFolderPath + "/" + FPaths::GetCleanFilename(URL);
	if (bIsStatic)
	{
		ImageFolderPath = FPaths::ProjectDir() + STATIC_PATH;
		ImageFileName = ImageFolderPath + "/" + FPaths::GetCleanFilename(URL);
		if (!PlatformFile.DirectoryExists(*ImageFolderPath))
			PlatformFile.CreateDirectory(*ImageFolderPath);
	}

	if (bNeedCover && PlatformFile.FileExists(*ImageFileName))
	{
		PlatformFile.DeleteFile(*ImageFileName);
	}

	//查找ImageFolderPath下图片 如果找到就加载图片，未找到就下载 
	FImageDirectoryVisitor TemplateDirectoryVisitor;
	PlatformFile.IterateDirectoryRecursively(*ImageFolderPath, TemplateDirectoryVisitor);

	if (!bIsStatic)//同时删除DynamicImage下多余日期文件夹
	{
		for (FString Directory : TemplateDirectoryVisitor.Directories)
		{
			if (FPaths::ConvertRelativePathToFull(Directory) != FPaths::ConvertRelativePathToFull(NewImageFolderPath))
			{
				if (!IFileManager::Get().DeleteDirectory(*FPaths::ConvertRelativePathToFull(Directory), true, true))
				{
					UE_LOG(LogTemp, Error, TEXT("DeleteDirectory Faild ,Path : %s"), *FPaths::ConvertRelativePathToFull(Directory));
				}
			}
		}
	}
	if (TemplateDirectoryVisitor.Files.Contains(ImageFileName))
	{
		//直接加载图片
		auto Texture = LoadImageToTextureByPath(ImageFileName);
		if (!Texture)
		{

			return;

		}

		m_Width = Texture->GetSurfaceWidth();
		m_Height = Texture->GetSurfaceHeight();
		if (bAutoSetImage)
		{
			SetBrushFromTextureDynamic(Texture, false);
		}
	}
	else
	{
		SendRequest(URL);
	}
}






void UHttpImage::SendRequest(const FString url)
{
	ImageUrl = url;
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UHttpImage::OnResponseReceived);
	request->SetURL(url);
	request->SetVerb("GET");
	if (bHaveToken)
	{
		request->SetHeader(TEXT("token"), TokenString);
	}

	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
}

void UHttpImage::OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	if (bWasSuccessful && response.IsValid() && response->GetContentLength() > 0)
	{
		FString Content = response->GetContentAsString();
		//储存前先转化为绝对路径
		ImageFileName = FPaths::ConvertRelativePathToFull(ImageFileName);
		//保存到图片为ImageFileName
		if (FFileHelper::SaveArrayToFile(response->GetContent(), *ImageFileName))
		{
			auto Texture = DataToTexture(response->GetContent());
			if (bAutoSetImage)
			{
				//加载图片
				SetBrushFromTextureDynamic(Texture, this->bMatchSize);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Image Save Faild ,URL : %s"), *ImageUrl);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Http Request Faild ,URL : %s"), *ImageUrl);
	}
}

static void WriteRawToTexture_RenderThread(FTexture2DDynamicResource* TextureResource, const TArray<uint8>& RawData, bool bUseSRGB = true)
{
	check(IsInRenderingThread());

	FTextureRHIRef TextureRHI = TextureResource->GetTexture2DRHI();

	int32 Width = TextureRHI->GetSizeX();
	int32 Height = TextureRHI->GetSizeY();

	uint32 DestStride = 0;
	uint8* DestData = reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_WriteOnly, DestStride, false, false));

	for (int32 y = 0; y < Height; y++)
	{
		uint8* DestPtr = &DestData[(Height - 1 - y) * DestStride];

		const FColor* SrcPtr = &((FColor*)(RawData.GetData()))[(Height - 1 - y) * Width];
		for (int32 x = 0; x < Width; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = SrcPtr->A;
			SrcPtr++;
		}
	}

	RHIUnlockTexture2D(TextureRHI, 0, false, false);
}

UTexture2DDynamic* UHttpImage::LoadImageToTextureByPath(const FString ImageFilePath)
{
	TArray<uint8> DataArray;
	if (FFileHelper::LoadFileToArray(DataArray, *ImageFilePath))
	{
		return DataToTexture(DataArray);
	}

	UE_LOG(LogTemp, Error, TEXT("LoadImageToTextureByPath Faild ,ImageFilePath : %s"), *ImageFilePath);
	return nullptr;
}

UTexture2DDynamic* UHttpImage::DataToTexture(const TArray<uint8> Data)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrappers[3] =
	{
		ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
	};

	for (auto ImageWrapper : ImageWrappers)
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Data.GetData(), Data.Num()))
		{
			TArray<uint8> RawData;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
			{
				if (UTexture2DDynamic* Texture = UTexture2DDynamic::Create(ImageWrapper->GetWidth(), ImageWrapper->GetHeight()))
				{
					Texture->SRGB = true;
					Texture->UpdateResource();

					FTexture2DDynamicResource* ParmTextureResource = static_cast<FTexture2DDynamicResource*>(Texture->GetResource());
					//TArray<uint8> ParamRawData = RawData;
					ENQUEUE_RENDER_COMMAND(FWriteRawDataToTexture)(
						[ParmTextureResource, RawData](FRHICommandListImmediate& RHICmdList)
						{
							WriteRawToTexture_RenderThread(ParmTextureResource, RawData);
						});

					m_Width = Texture->GetSurfaceWidth();
					m_Height = Texture->GetSurfaceHeight();
					OnImageReceived.Broadcast(Texture);
					return Texture;
				}
			}
		}
	}

	UE_LOG(LogTemp, Error, TEXT("DataToTexture Faild"));
	return nullptr;
}

#undef LOCTEXT_NAMESPACE

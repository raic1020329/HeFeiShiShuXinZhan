// Fill out your copyright notice in the Description page of Project Settings.

#include "Http/SCDataStreamHttpRequest.h"
#include "HttpModule.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "DTGameplay/DTCommonFunLibrary.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

USCDataStreamHttpRequest* USCDataStreamHttpRequest::AsyncHttpRequestAction(const FString& URL,
	const FString& JsonContent,
	const TMap<FString, FString> HeadMap,
	EHttpJsonVerb Verb,
	EHttpContentType InContentType,
	const FString& InStructName,
	UObject* InObejectTarget,
	EDataType InDataType)
{
	USCDataStreamHttpRequest* ThisObject = NewObject<USCDataStreamHttpRequest>();
	if (ThisObject)
	{
		ThisObject->RequestData(URL, JsonContent, HeadMap, Verb, InContentType,InStructName, InObejectTarget, InDataType);
	}
	return ThisObject;
}

void USCDataStreamHttpRequest::RequestData(const FString& URL, const FString& JsonContent, const TMap<FString, FString> HeadMap, EHttpJsonVerb Verb, EHttpContentType InContentType, const FString& InStructName, UObject* InObejectTarget, EDataType InDataType /*= EDataType::Http*/)
{
	m_Object = InObejectTarget;
	m_StructName = InStructName;
	m_DataType = InDataType;
	m_url = URL;
	if (InDataType == EDataType::HttpOnly|| InDataType == EDataType::Auto)
	{
		FString V;
		switch (Verb)
		{
		case EHttpJsonVerb::GET:
			V = TEXT("GET");
			break;
		case EHttpJsonVerb::POST:
			V = TEXT("POST");
			break;
		}

		FString contentType;
		switch (InContentType)
		{
		case EHttpContentType::x_www_form_urlencoded_url:
			contentType = TEXT("application/x-www-form-urlencoded;charset=utf-8");
			break;
		case EHttpContentType::x_www_form_urlencoded_body:
			contentType = TEXT("application/x-www-form-urlencoded;charset=utf-8");
			break;
		case EHttpContentType::json:
			contentType = TEXT("application/json;charset=utf-8");
			break;
		case EHttpContentType::binary:
			contentType = TEXT("application/octet-stream;charset=utf-8");
			break;
		}
		progress(URL, JsonContent, HeadMap, V, contentType);
	}
	else if(InDataType==EDataType::LocalOnly)
	{
		TFunction<void()> LamamFun = [&, this]()
		{
			
			FGraphEventRef EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[&,this]
			{
				ReadLocalData(this->m_StructName,this->m_url);

			}, TStatId(),
				nullptr,
				ENamedThreads::GameThread//切换到游戏线程
				);

			FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);
		};
		auto Task = new FAutoDeleteAsyncTask<FAutoAnsyTask>(LamamFun);
		Task->StartBackgroundTask();
	}	
}

void USCDataStreamHttpRequest::ReadLocalData(const FString& InStructName, const FString& URL)
{
	FString CurrentFilePath = "";
	if (URL.IsEmpty())
		CurrentFilePath = FPaths::ProjectContentDir() + TEXT("Config/") + InStructName + TEXT(".json");
	else
		CurrentFilePath = URL + TEXT(".json");
	CurrentFilePath = FPaths::ConvertRelativePathToFull(CurrentFilePath);
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*CurrentFilePath))
	{
		FString Content;
		FFileHelper::LoadFileToString(Content, *CurrentFilePath);
		if (UDTCommonFunLibrary::AutoParserJson(Content, m_StructName, m_Object))
		{
			OnSuccess.Broadcast(Content);
		}
		else
		{
			OnFail.Broadcast(TEXT("Local data is not in json format"));
		}
	}
	else
	{
		FString Msg = TEXT("The current file does not exist path:");
		Msg.Append(CurrentFilePath);
		OnFail.Broadcast(Msg);
	}

}

void USCDataStreamHttpRequest::progress(const FString& URL, const FString& JsonContent, const TMap<FString, FString> HeadMap, const FString& Verb, const FString& InContentType)
{
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	if (Request)
	{
		Request->SetURL(URL);
		Request->SetVerb(Verb);
		Request->SetHeader("Content-Type", InContentType);
		for (auto Item : HeadMap)
		{
			Request->SetHeader(Item.Key, Item.Value);
		}
		Request->SetContentAsString(JsonContent);
		Request->OnProcessRequestComplete().BindUObject(this, &USCDataStreamHttpRequest::OnProcessRequestComplete);
		Request->ProcessRequest();
	}
}

void USCDataStreamHttpRequest::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!Response.IsValid())
	{
		if (m_DataType == EDataType::Auto)
		{
			ReadLocalData(m_StructName);
		}
		else
		{
			const FString& Msg = TEXT("Response is nullptr!");
			OnFail.Broadcast(Msg);
		}
		return;
	}
	//400-499     500-599
	//
	int32 Code = Response->GetResponseCode();
	if (!bWasSuccessful)
	{
		if (m_DataType == EDataType::Auto)
		{
			ReadLocalData(m_StructName);
		}
		else
		{
			const FString& Msg = FString::Printf(TEXT("Request failed(Code=%d)"), Code);
			OnFail.Broadcast(Msg);
		}
		return;
	}
	//200-299
	if (EHttpResponseCodes::IsOk(Code))
	{
		FString OutputString = Response->GetContentAsString();
		if (m_StructName.IsEmpty() || !m_Object)
		{
			OnFail.Broadcast(TEXT("StrucName or InObejectTarget can not nullptr"));
		}
		else
		{
			if (UDTCommonFunLibrary::AutoParserJson(OutputString, m_StructName, m_Object))
			{
				OnSuccess.Broadcast(OutputString);
			}
			else
			{
				if (m_DataType == EDataType::Auto)
				{
					ReadLocalData(m_StructName);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Data reflection failed! And See Details  Msg"));
					OnFail.Broadcast(OutputString);
				}		
			}
		}
	}
	else
	{
		if (m_DataType == EDataType::Auto)
		{
			ReadLocalData(m_StructName);
		}
		else
		  OnFail.Broadcast(TEXT("Unknow Fail"));
	}

}
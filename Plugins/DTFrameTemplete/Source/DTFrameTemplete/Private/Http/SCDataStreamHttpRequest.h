
#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Async/AsyncWork.h"
#include "Async/TaskGraphInterfaces.h"
#include "SCDataStreamHttpRequest.generated.h"

class FAutoAnsyTask :public FNonAbandonableTask
{
public:

	FAutoAnsyTask(TFunction<void()>& InFuntion)
		:Funtion(InFuntion)
	{
	}
	void DoWork()
	{
		Funtion();		
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAutoAnsyTask, STATGROUP_ThreadPoolAsyncTasks);
	}
private:
	TFunction<void()> Funtion;
};

UENUM(BlueprintType)
enum class EDataType:uint8
{
	LocalOnly,//只读本地数据
	HttpOnly,//只获取网络数据，不获取本地
	Auto//有网情况下获取网络数据，没网获取本地的
};

UENUM(BlueprintType)
enum class EHttpContentType :uint8
{
	x_www_form_urlencoded_url	UMETA(DisplayName = "x-www-form-urlencoded (URL)"),
	x_www_form_urlencoded_body	UMETA(DisplayName = "x-www-form-urlencoded (Request Body)"),
	json,
	binary
};

UENUM(BlueprintType)
enum class EHttpJsonVerb : uint8
{
	GET,
	POST
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDataHttpRequestDelegate,const FString&, Msg);
UCLASS(BlueprintType)
class DTFRAMETEMPLETE_API USCDataStreamHttpRequest :public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "SCDataStreamHttpRequest")
	static USCDataStreamHttpRequest* AsyncHttpRequestAction(
		const FString& URL, 
		const FString& JsonContent, 
		const TMap<FString, FString> HeadMap,  
		EHttpJsonVerb Verb,
		EHttpContentType InContentType,
		const FString& InStructName,
		UObject* InObejectTarget,
		EDataType InDataType = EDataType::HttpOnly);

	UPROPERTY(BlueprintAssignable, Category = "SCDataStreamHttpRequest")
	FDataHttpRequestDelegate OnSuccess;
	UPROPERTY(BlueprintAssignable, Category = "SCDataStreamHttpRequest")
	FDataHttpRequestDelegate OnFail;
private: 
	void RequestData(const FString& URL,const FString& JsonContent,const TMap<FString, FString> HeadMap,EHttpJsonVerb Verb, EHttpContentType InContentType, const FString& InStructName,UObject* InObejectTarget,EDataType InDataType = EDataType::HttpOnly);
	void ReadLocalData(const FString& InStructName, const FString& URL = "");
	void progress(const FString& URL, const FString& JsonContent, const TMap<FString, FString> HeadMap,const FString& Verb, const FString& InContentType);
	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	
private:
	FString m_StructName;
	UObject* m_Object;
	EDataType m_DataType;
	FString m_url;
};

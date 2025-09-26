// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DTControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDTControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DTFRAMETEMPLETE_API IDTControlInterface
{
	GENERATED_BODY()
public:
	static void Execute_DoCloseUI(UObject* InObj);
	virtual void DoCloseUI_Implementation() = 0;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DTControlInterface")
	void ReleaseOtherRes();
	virtual void ReleaseOtherRes_Implementation() = 0;
	UFUNCTION(BlueprintImplementableEvent, Category = "DTControlInterface")
	void ReturnParentUIEvent();	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DTControlInterface")
	void NotifyIsOpenInfo(bool bIsInfo);
	virtual void NotifyIsOpenInfo_Implementation(bool bIsInfo) {};
};

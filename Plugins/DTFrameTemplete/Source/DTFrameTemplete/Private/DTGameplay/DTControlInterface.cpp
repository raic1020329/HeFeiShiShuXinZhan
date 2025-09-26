// Fill out your copyright notice in the Description page of Project Settings.


#include "DTGameplay/DTControlInterface.h"

// Add default functionality here for any IDTControlInterface functions that are not pure virtual.

void IDTControlInterface::Execute_DoCloseUI(UObject* InObj)
{
	if (InObj != NULL && InObj->GetClass()->ImplementsInterface(UDTControlInterface::StaticClass()))
	{
		UFunction* const Func = InObj->FindFunction(TEXT("DoCloseUI"));
		if (Func)
		{
			InObj->ProcessEvent(Func, NULL);
		}
		if (auto I = (IDTControlInterface*)(InObj->GetNativeInterfaceAddress(UDTControlInterface::StaticClass())))
		{
			I->DoCloseUI_Implementation();
		}
	}
	
}
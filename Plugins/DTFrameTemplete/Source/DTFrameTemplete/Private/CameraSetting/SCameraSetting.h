// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
/**
 * 
 */
class DTFRAMETEMPLETE_API SCameraSetting:public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SCameraSetting)
	{

	}
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);
};

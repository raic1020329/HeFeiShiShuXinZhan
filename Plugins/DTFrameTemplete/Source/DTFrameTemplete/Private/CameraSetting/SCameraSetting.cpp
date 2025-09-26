// Fill out your copyright notice in the Description page of Project Settings.


#include "SCameraSetting.h"
#include "Widgets/Images/SImage.h"
void SCameraSetting::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SImage)
		];
}

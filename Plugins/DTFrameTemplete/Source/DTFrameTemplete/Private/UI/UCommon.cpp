// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UCommon.h"
#include "Components/TextBlock.h"

void UCommon::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (TextBlock_Unit)
		TextBlock_Unit->SetVisibility(bShowUnit ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UCommon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCommon::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCommon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCommon::initText(UTextBlock* _Textblock, const FText& _Text, const FSlateColor& _Color, const FSlateFontInfo& _Font)
{
	if (_Textblock)
	{
		_Textblock->SetText(_Text);
		_Textblock->SetColorAndOpacity(_Color);
		_Textblock->SetFont(_Font);
	}
}

void UCommon::initProperty()
{
	if (TextBlock_Key)
		initText(TextBlock_Key, KeyText, KeyColor, KeyFont);
	if (TextBlock_Value)
		initText(TextBlock_Value, ValueText, ValueColor, ValueFont);
	if (TextBlock_Unit)
		initText(TextBlock_Unit, UnitText, UnitColor, UnitFont);
}

void UCommon::UpdateData_Implementation(const FText& _Text)
{
	if (TextBlock_Value)
		TextBlock_Value->SetText(_Text);
}

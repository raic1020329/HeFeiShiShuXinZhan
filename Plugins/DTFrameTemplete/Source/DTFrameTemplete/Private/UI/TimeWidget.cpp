// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/TimeWidget.h"
#include "DTGameplay/DTCommonFunLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"


void UTimeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* world = GetWorld();
	if (world)
	{
		world->GetTimerManager().SetTimer(TimeHandle, this, &UTimeWidget::GetTime, TimerChecked, true);
	}
}

void UTimeWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTimeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTimeWidget::GetTime()
{
	FDateTime data = UKismetMathLibrary::Now();
	if (TextBlock_Date)
	{
		//FText text = FText::Format(LOCTEXT("ExampleFormat", "{0} 年 {1} 月 {2} 日"), FText::AsNumber(data.GetYear()), FText::AsNumber(data.GetMonth()), FText::AsNumber(data.GetDay()));

		FString year = FString::FromInt(data.GetYear());
		FString month = FString::FromInt(data.GetMonth());
		FString day = FString::FromInt(data.GetDay());
		FText MyText = FText::FromString(year + TEXT("年") + month + TEXT("月") + day + TEXT("日"));
		TextBlock_Date->SetText(MyText);
	}
	if (TextBlock_Time)
	{
		FString hour = data.GetHour() < 10 ? "0" + FString::FromInt(data.GetHour()) : FString::FromInt(data.GetHour());
		FString minute = data.GetMinute() < 10 ? "0" + FString::FromInt(data.GetMinute()) : FString::FromInt(data.GetMinute());
		FString second = data.GetSecond() < 10 ? "0" + FString::FromInt(data.GetSecond()) : FString::FromInt(data.GetSecond());
		FText MyText = FText::FromString(hour + ":" + minute + ":" + second);
		TextBlock_Time->SetText(MyText);
	}
	if (TextBlock_Week)
	{
		FString text = FString(TEXT("星期"));
		int weekIndex = UDTCommonFunLibrary::ZellerCongruence(data);
		FString week = "";
		switch (weekIndex)
		{
		case 0:
		{
			week = TEXT("日");
			break;
		}
		case 1:
		{
			week = TEXT("一");
			break;
		}
		case 2:
		{
			week = TEXT("二");
			break;
		}
		case 3:
		{
			week = TEXT("三");
			break;
		}
		case 4:
		{
			week = TEXT("四");
			break;
		}
		case 5:
		{
			week = TEXT("五");
			break;
		}
		case 6:
		{
			week = TEXT("六");
			break;
		}
		case 7:
		{
			week = TEXT("日");
			break;
		}
		default:
			break;
		}
		FText MyText = FText::FromString(text + week);
		TextBlock_Week->SetText(MyText);
	}
}

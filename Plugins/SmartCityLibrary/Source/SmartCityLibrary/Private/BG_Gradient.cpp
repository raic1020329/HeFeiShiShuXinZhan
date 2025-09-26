#include "BG_Gradient.h"
#include "Kismet/GameplayStatics.h"
#include "Rendering/DrawElements.h"





UBG_Gradient::UBG_Gradient(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	
}

void UBG_Gradient::SetBGStyle(const FVector2D InContentSize, FDashedLineStyle InDashLineStyle, int32 SectionNum_X, int32 SectionNum_Y)
{
	if (!InDashLineStyle.CreateDashLine)
		return;
	ContentSize = InContentSize;
	DashLineStyle = InDashLineStyle;
	DashedLineList.Empty();
	if (DashLineStyle.DashLineThickness <= 0 || DashLineStyle.DashDensity <= 0)
		return;

	float Ratio = FMath::Clamp(DashLineStyle.DashRatio, 0.f, 1.f);
	float PreIntervalLength;
	float PreDashLineLength;
	if (DashLineStyle.DashDensity == 1)
	{
		PreDashLineLength = ContentSize.X;
		PreIntervalLength = 0;
	}
	else
	{
		PreIntervalLength = ContentSize.X*Ratio / (DashLineStyle.DashDensity - 1);
		PreDashLineLength = ContentSize.X*(1 - Ratio) / DashLineStyle.DashDensity;
	}
		
	//float PerLine = PreDashLineLength + PreIntervalLength;
	if (DashLineStyle.ShowHorizontal)
	{
		int32 Section_Y = DashLineStyle.CreateAssiztLine ? SectionNum_Y * 2 : SectionNum_Y;
		float PreStep_Y = ContentSize.Y / Section_Y;
		for (int32 i = 0; i < Section_Y; i++)
		{

			for (float length = 0; length < ContentSize.X; length += PreIntervalLength)
			{
				FVector2D StartPoint = FVector2D(length, PreStep_Y*i);
				length = FMath::Min(length + PreDashLineLength, ContentSize.X);
				FVector2D EndPoint = FVector2D(length, PreStep_Y*i);
				FDashedData DashedPoint;
				DashedPoint.StartPoint = StartPoint;
				DashedPoint.EndPoint = EndPoint;
				DashedPoint.Thickness = (DashLineStyle.CreateAssiztLine&&i % 2 == 1) ? DashLineStyle.DashLineThickness * 0.8 : DashLineStyle.DashLineThickness;
				DashedPoint.LineColor = (DashLineStyle.CreateAssiztLine&&i % 2 == 1) ? DashLineStyle.DashLineColor * 0.8 : DashLineStyle.DashLineColor;
				DashedLineList.Add(DashedPoint);
			}
		}
	}
	if (DashLineStyle.ShowVertical)
	{
		int32 Section_X = DashLineStyle.CreateAssiztLine ? SectionNum_X * 2 : SectionNum_X;
		float PreStep_X = ContentSize.X / Section_X;
		for (int32 i = 0; i < Section_X + 1; i++)
		{
			
			for (float length = 0; length < ContentSize.Y; length += PreIntervalLength)
			{
				FVector2D StartPoint = FVector2D(PreStep_X*i, length);
				length = FMath::Min(length + PreDashLineLength, ContentSize.Y);
				FVector2D EndPoint = FVector2D(PreStep_X*i, length);
				FDashedData DashedPoint;
				DashedPoint.StartPoint = StartPoint;
				DashedPoint.EndPoint = EndPoint;
				DashedPoint.Thickness = (DashLineStyle.CreateAssiztLine&&i % 2 == 1) ? DashLineStyle.DashLineThickness * 0.8 : DashLineStyle.DashLineThickness;
				DashedPoint.LineColor = (DashLineStyle.CreateAssiztLine&&i % 2 == 1) ? DashLineStyle.DashLineColor * 0.8 : DashLineStyle.DashLineColor;
				DashedLineList.Add(DashedPoint);
			}
		
		}
	}



}


void UBG_Gradient::SetAverageValue(float AvgValue)
{
	AvgLineList.Empty();

	if (DashLineStyle.DashLineThickness <= 0 || DashLineStyle.DashDensity <= 0)
		return;

	if (DashLineStyle.CreateAverageLine)
	{

		float Ratio = FMath::Clamp(DashLineStyle.DashRatio, 0.f, 1.f);
		float PreIntervalLength;
		float PreDashLineLength;
		if (DashLineStyle.DashDensity == 1)
		{
			PreDashLineLength = ContentSize.X;
			PreIntervalLength = 0;
		}
		else
		{
			PreIntervalLength = ContentSize.X*Ratio / (DashLineStyle.DashDensity - 1);
			PreDashLineLength = ContentSize.X*(1 - Ratio) / DashLineStyle.DashDensity;
		}
		if (DashLineStyle.bBeUsedHorizontal)
		{
			for (float length = 0; length < ContentSize.X; length += PreIntervalLength)
			{
				FVector2D StartPoint = FVector2D(length, (1 - AvgValue)*ContentSize.Y);
				length = FMath::Min(length + PreDashLineLength, ContentSize.X);
				FVector2D EndPoint = FVector2D(length, (1 - AvgValue)*ContentSize.Y);
				FDashedData AvgPoint;
				AvgPoint.StartPoint = StartPoint;
				AvgPoint.EndPoint = EndPoint;
				AvgPoint.Thickness = DashLineStyle.DashLineThickness;
				AvgPoint.LineColor = DashLineStyle.AverageLineColor;
				AvgLineList.Add(AvgPoint);
			}
		}
		else
		{
			for (float length = 0; length < ContentSize.Y; length += PreIntervalLength)
			{
				FVector2D StartPoint = FVector2D(AvgValue*ContentSize.X, length);
				length = FMath::Min(length + PreDashLineLength, ContentSize.Y);
				FVector2D EndPoint = FVector2D(AvgValue*ContentSize.X, length);
				FDashedData AvgPoint;
				AvgPoint.StartPoint = StartPoint;
				AvgPoint.EndPoint = EndPoint;
				AvgPoint.Thickness = DashLineStyle.DashLineThickness;
				AvgPoint.LineColor = DashLineStyle.AverageLineColor;
				AvgLineList.Add(AvgPoint);
			}
		}
	}

}

void UBG_Gradient::DrawBG(FPaintContext  Context)
{
	Context.MaxLayer++;
	if (DashedLineList.Num() > 0)
	{
		for (int32 i = 0; i < DashedLineList.Num(); i++)
		{
			TArray<FVector2D> Points;
			Points.Add(DashedLineList[i].StartPoint);
			Points.Add(DashedLineList[i].EndPoint);
			FSlateDrawElement::MakeLines(
				Context.OutDrawElements,
				Context.MaxLayer,
				Context.AllottedGeometry.ToPaintGeometry(),
				Points,
				ESlateDrawEffect::None,
				DashedLineList[i].LineColor,
				true,
				DashedLineList[i].Thickness
			);
		}
	}

	if (AvgLineList.Num() > 0)
	{
		for (int32 i = 0; i < AvgLineList.Num(); i++)
		{
			TArray<FVector2D> Points;
			Points.Add(AvgLineList[i].StartPoint);
			Points.Add(AvgLineList[i].EndPoint);
			FSlateDrawElement::MakeLines(
				Context.OutDrawElements,
				Context.MaxLayer,
				Context.AllottedGeometry.ToPaintGeometry(),
				Points,
				ESlateDrawEffect::None,
				AvgLineList[i].LineColor,
				true,
				AvgLineList[i].Thickness
			);
		}
	}
}
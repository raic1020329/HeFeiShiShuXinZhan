#include "CubicCanvasPanel.h"
#include "Rendering/DrawElements.h"


UCubicCanvasPanel::UCubicCanvasPanel(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	
}

TArray<float> UCubicCanvasPanel::TurnDataToLineDataPoint(TMap<FString, float> LineData, TArray<FLinearColor> InColorList, const FVector2D ContentSize, float Thickness, float PointDensity)
{

	CubicThickness = Thickness;

	//根据传入的点密度PointDensity计算并添加背景点位置；
	PointsList.Empty();
	Step_X = ContentSize.X / PointDensity;
	Step_Y = ContentSize.Y / PointDensity;
	for (float PointY = Step_Y; PointY < ContentSize.Y; PointY += Step_Y)
	{
		for (float PointX = Step_X; PointX < ContentSize.X; PointX += Step_X)
			PointsList.Add(FVector2D(PointX, PointY));
	}

	Keys.Empty();
	LineData.GetKeys(Keys);
		
	ColorMap.Empty();
	for (int32 i = 0; i < Keys.Num(); i++)
	{
		if (i < InColorList.Num())
			ColorMap.Add(Keys[i], InColorList[i]);
		else
			ColorMap.Add(Keys[i], FLinearColor::White);
	}
	TArray<float> ValueArray;
	LineData.GenerateValueArray(ValueArray);
	BubbleAlgorithm(ValueArray, Keys);


	TArray<float> PreItemRatio;
	CalcSectionRatio(ValueArray, PreItemRatio);

	int32 ItemNum = PreItemRatio.Num();

	CubicCenterPoints.Empty();
	AllCubicPoints.Empty();
	
	for (int32 i = 0; i < ItemNum; i++)
	{
		TArray<FVector2D> PreCubicPoints;
		FCubicPoint PreCubic;
		float Offset_X, Offset_Y;
		Offset_X = Offset_Y = i == (ItemNum - 1) ? 0 : 1;
		float CenterX, CenterY;
		if (i == 0)
		{
			
			PreCubicPoints.Add(FVector2D(0.f, 0.f));
			PreCubicPoints.Add(FVector2D(0.f,ContentSize.Y));
			PreCubicPoints.Add(FVector2D(floor(PreItemRatio[0] * ContentSize.X - Offset_X), ContentSize.Y));
			PreCubicPoints.Add(FVector2D(floor(PreItemRatio[0] * ContentSize.X - Offset_X), 0.f));
			PreCubic.CubicDataPoints = PreCubicPoints;

			AllCubicPoints.Add(PreCubic);
		}

		else if (i == 1)
		{
			
			PreCubicPoints.Add(FVector2D(ceil(PreItemRatio[0] * ContentSize.X + 1), 0.f));
			PreCubicPoints.Add(FVector2D(ceil(PreItemRatio[0] * ContentSize.X + 1), floor(PreItemRatio[1] * ContentSize.Y - Offset_Y)));
			PreCubicPoints.Add(FVector2D(ContentSize.X, floor(PreItemRatio[1] * ContentSize.Y - Offset_Y)));
			PreCubicPoints.Add(FVector2D(ContentSize.X, 0.f));
			
			PreCubic.CubicDataPoints = PreCubicPoints;
			AllCubicPoints.Add(PreCubic);
		}
		else if (i == 2)
		{
			
			PreCubicPoints.Add(FVector2D(ceil(PreItemRatio[0] * ContentSize.X + 1), ceil(PreItemRatio[1] * ContentSize.Y + 1)));
			PreCubicPoints.Add(FVector2D(ceil(PreItemRatio[0] * ContentSize.X + 1), ContentSize.Y));
			PreCubicPoints.Add(FVector2D(floor((PreItemRatio[2] * (1 - PreItemRatio[0]) + PreItemRatio[0])* ContentSize.X - Offset_X), ContentSize.Y));
			PreCubicPoints.Add(FVector2D(floor((PreItemRatio[2] * (1 - PreItemRatio[0]) + PreItemRatio[0])* ContentSize.X - Offset_X), ceil(PreItemRatio[1] * ContentSize.Y + 1)));		

			PreCubic.CubicDataPoints = PreCubicPoints;
			AllCubicPoints.Add(PreCubic);
		}
		else 
		{

			PreCubicPoints.Add(FVector2D(AllCubicPoints[i - 1].CubicDataPoints[3].X + 2, AllCubicPoints[i - 1].CubicDataPoints[3].Y));
			PreCubicPoints.Add(FVector2D(AllCubicPoints[i - 1].CubicDataPoints[3].X + 2, ContentSize.Y));
			PreCubicPoints.Add(FVector2D(floor(PreItemRatio[i] * (ContentSize.X - AllCubicPoints[i - 1].CubicDataPoints[3].X) + AllCubicPoints[i - 1].CubicDataPoints[3].X - Offset_X), ContentSize.Y));
			PreCubicPoints.Add(FVector2D(floor(PreItemRatio[i] * (ContentSize.X - AllCubicPoints[i - 1].CubicDataPoints[3].X) + AllCubicPoints[i - 1].CubicDataPoints[3].X - Offset_X), AllCubicPoints[i - 1].CubicDataPoints[3].Y));

			PreCubic.CubicDataPoints = PreCubicPoints;
			AllCubicPoints.Add(PreCubic);
				
		}

		CenterX = (PreCubicPoints[0].X + PreCubicPoints[3].X) / 2;
		CenterY = (PreCubicPoints[0].Y + PreCubicPoints[1].Y) / 2;
		CubicCenterPoints.Add(FVector2D(CenterX, CenterY));
	
	}


	return ValueArray;

}

void UCubicCanvasPanel::DrawCubic(FPaintContext Context)
{
	if (AllCubicPoints.Num() > 0)
	{
Context.MaxLayer++;

for (int32 i = 0; i < AllCubicPoints.Num(); i++)
{
	//FLinearColor CubicColor = i < ColorList.Num() ? ColorList[i] : FLinearColor::White;
	FLinearColor CubicColor = *(ColorMap.Find(Keys[i]));
	for (int32 j = 0; j < 4; j++)
	{

		TArray<FVector2D> Points;
		Points.Add(AllCubicPoints[i].CubicDataPoints[j]);
		if (j == 3)
			Points.Add(AllCubicPoints[i].CubicDataPoints[0]);
		else
			Points.Add(AllCubicPoints[i].CubicDataPoints[j + 1]);

		FSlateDrawElement::MakeLines(
			Context.OutDrawElements,
			Context.MaxLayer,
			Context.AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			CubicColor,
			true,
			CubicThickness
		);
	}

	for (int32 k = 0; k < PointsList.Num(); k++)
	{
		if (PointsList[k].X > AllCubicPoints[i].CubicDataPoints[0].X + Step_X * 0.2
			&& PointsList[k].X < AllCubicPoints[i].CubicDataPoints[3].X - Step_X * 0.2
			&& PointsList[k].Y > AllCubicPoints[i].CubicDataPoints[0].Y + Step_Y * 0.2
			&& PointsList[k].Y < AllCubicPoints[i].CubicDataPoints[1].Y - Step_Y * 0.2
			&& (PointsList[k] - CubicCenterPoints[i]).Size()> Step_X)
		{
			for (int32 j = 0; j < 2; j++)
			{
				TArray<FVector2D> Points;
				if (j == 0)
				{
					Points.Add(FVector2D(PointsList[k].X - 1, PointsList[k].Y));
					Points.Add(FVector2D(PointsList[k].X + 1, PointsList[k].Y));
				}
				else
				{
					Points.Add(FVector2D(PointsList[k].X, PointsList[k].Y - 1));
					Points.Add(FVector2D(PointsList[k].X, PointsList[k].Y + 1));
				}
				FSlateDrawElement::MakeLines(
					Context.OutDrawElements,
					Context.MaxLayer,
					Context.AllottedGeometry.ToPaintGeometry(),
					Points,
					ESlateDrawEffect::None,
					CubicColor,
					true,
					CubicThickness
				);
			}
		}
	}
}

	}
}

TArray<FVector2D> UCubicCanvasPanel::GetCubicCenterPoints()
{
	return CubicCenterPoints;
}

void UCubicCanvasPanel::BubbleAlgorithm(TArray<float>& ValueArray_, TArray<FString>& KeyArray_)
{
	for (int32 i = 0; i < ValueArray_.Num() - 1; i++)
	{
		for (int32 j = 0; j < ValueArray_.Num() - 1 - i; j++)
		{
			if (ValueArray_[j] < ValueArray_[j + 1])
			{
				float ValueTemp = ValueArray_[j];
				ValueArray_[j] = ValueArray_[j + 1];
				ValueArray_[j + 1] = ValueTemp;

				FString KeyTemp = KeyArray_[j];
				KeyArray_[j] = KeyArray_[j + 1];
				KeyArray_[j + 1] = KeyTemp;
			}
		}
	}

}

TArray<float> UCubicCanvasPanel::BubbleAlgorithm_float(TArray<float> inArray)
{
	for (int32 i = 0; i < inArray.Num() - 1; i++)
	{
		for (int32 j = 0; j < inArray.Num() - 1 - i; j++)
		{
			if (inArray[j] < inArray[j + 1])
			{
				float ValueTemp = inArray[j];
				inArray[j] = inArray[j + 1];
				inArray[j + 1] = ValueTemp;
			}
		}
	}
	return inArray;
}

void UCubicCanvasPanel::CalcSectionRatio(TArray<float>& inArray, TArray<float>& OutArray)
{
	float Sum = 0;
	for (int32 i = 0; i < inArray.Num(); i++)
	{
		Sum += inArray[i];
	}
	float Sum_ = Sum;
	for (int32 i = 0; i < inArray.Num(); i++)
	{
		float Percent = inArray[i] / Sum_;
		OutArray.Add(Percent);
		Sum_ -= inArray[i];
		if (i == inArray.Num() - 1)
		{
			float EndValue = 1.f;
			for (int32 j = 0; j < inArray.Num() - 1; j++)
			{
				EndValue -= inArray[j];
			}
			inArray[i] = EndValue;
		}			
		else
			inArray[i] = ((int)(inArray[i] / Sum * 100 + 0.5)) / 100.f;
	}

}

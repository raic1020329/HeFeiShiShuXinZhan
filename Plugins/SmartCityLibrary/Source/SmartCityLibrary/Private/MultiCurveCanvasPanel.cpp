#include "MultiCurveCanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Rendering/DrawElements.h"


UMultiCurveCanvasPanel::UMultiCurveCanvasPanel(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	FloatCurve = new(FRichCurve);
	//CurvePointIndex = 0;
}
void UMultiCurveCanvasPanel::DrawCurve(FPaintContext  Context)
{

	if (LineDataPoints.Num() > 0 && LineStyles.Num() > 0)
	{
		Context.MaxLayer++;
		/*if (bResetData||!bShowingEnd)
		{
			if (bResetData)
			{	
				MultiAllCurvePoints.Empty();
				MultiRebuildCurvePoints.Empty();
				
				for (int32 i = 0; i < LineDataPoints.Num(); i++)
				{

					TArray<FVector2D> CurvePoint;
					FCurvePointsArray Curve;
					MultiAllCurvePoints.Add(Curve);
					MultiRebuildCurvePoints.Add(Curve);
					for (int32 j = 0; j < LineDataPoints[i].CurvePoints.Num() - 1; j++)
					{
						CreateLinesForSegment(Context, LineDataPoints[i].CurvePoints[j], LineDataPoints[i].CurvePoints[j + 1], CurvePoint, i);
						MultiAllCurvePoints[i].CurvePoints += CurvePoint;
						CurvePoint.Empty();
					}
					
				}

				TargetPointStep = floor(MultiAllCurvePoints[0].CurvePoints.Num() / (ItemNum - 1));

				TargetPoint = TargetPointStep;

				if (!bRebuild)
				{

					TargetPoint = MultiAllCurvePoints[0].CurvePoints.Num();
					CurvePointIndex = TargetPoint - TargetPointStep - 1;
					for (int32 i = 0; i < LineDataPoints.Num(); i++)
					{					
						for (int32 j = 0; j < CurvePointIndex; j++)
							MultiRebuildCurvePoints[i].CurvePoints.Add(MultiAllCurvePoints[i].CurvePoints[j]);
					}
				}
					
				bResetData = false;
			}


			int32 PreAddPointNum;
			PreAddPointNum = FMath::Clamp(MultiAllCurvePoints[0].CurvePoints.Num()* UGameplayStatics::GetWorldDeltaSeconds(this) / LineStyles[0].LineAnimTime, 1.f, 20.f);

			for (int32 i = 0; i < PreAddPointNum && CurvePointIndex < MultiAllCurvePoints[0].CurvePoints.Num(); i++)
			{
				for (int32 j = 0; j < LineDataPoints.Num(); j++)
				{
					MultiRebuildCurvePoints[j].CurvePoints.Add(MultiAllCurvePoints[j].CurvePoints[CurvePointIndex]);
				}

				CurvePointIndex += 1;
				if (CurvePointIndex >= TargetPoint)
				{
					TargetPoint += TargetPointStep;
					OnTimeCall.Broadcast();
				}
			}
			
			for (int32 i = 0; i < LineStyles.Num() && i < LineDataPoints.Num(); i++)
			{

				if (MultiRebuildCurvePoints[i].CurvePoints.Num() > 2)
				{
					FSlateDrawElement::MakeLines(
						Context.OutDrawElements,
						Context.MaxLayer,
						Context.AllottedGeometry.ToPaintGeometry(),
						MultiRebuildCurvePoints[i].CurvePoints,
						ESlateDrawEffect::None,
						LineStyles[i].LineColor,
						true,
						LineStyles[i].Thickness
					);
				}
				GetBGImageTextureData(MultiRebuildCurvePoints[i].CurvePoints, i);
				if (i == LineStyles.Num() - 1 && CurvePointIndex >= MultiAllCurvePoints[i].CurvePoints.Num())
				{
					for (int32 j = 0; j < LineDataPoints.Num(); j++)
					{
						MultiRebuildCurvePoints.Empty();

						MultiAllCurvePoints[j].CurvePoints.RemoveAt(1);
						MultiAllCurvePoints[j].CurvePoints.RemoveAt(0);
						LineDataPoints[j].CurvePoints[0].X += 2;
					}

					CurvePointIndex = 0;
					bShowingEnd = true;
				}
			}


		}
		else
		{*/
			for (int32 i = 0; i < LineStyles.Num() && i < LineDataPoints.Num(); i++)
			{
				if (LineStyles[i].DashLine)
				{
					for (int32 j = 0; j < MultiAllCurvePoints[i].DashLineList.Num(); j++)
					{

						FSlateDrawElement::MakeLines(
							Context.OutDrawElements,
							Context.MaxLayer,
							Context.AllottedGeometry.ToPaintGeometry(),
							MultiAllCurvePoints[i].DashLineList[j].PreDashLine,
							ESlateDrawEffect::None,
							LineStyles[i].LineColor,
							true,
							LineStyles[i].Thickness
						);
					}
				}
				else
				{
					if (CurveLineType == ERichCurveInterpMode::RCIM_Cubic)
					{
						FSlateDrawElement::MakeLines(
							Context.OutDrawElements,
							Context.MaxLayer,
							Context.AllottedGeometry.ToPaintGeometry(),
							MultiAllCurvePoints[i].CurvePoints,
							ESlateDrawEffect::None,
							LineStyles[i].LineColor,
							true,
							LineStyles[i].Thickness
						);
					}
					else
					{
						FSlateDrawElement::MakeLines(
							Context.OutDrawElements,
							Context.MaxLayer,
							Context.AllottedGeometry.ToPaintGeometry(),
							LineDataPoints[i].CurvePoints,
							ESlateDrawEffect::None,
							LineStyles[i].LineColor,
							true,
							LineStyles[i].Thickness
						);
					}
				}
			}

		//}

	}

}


void UMultiCurveCanvasPanel::TurnDataToLineDataPoint(TArray<FLineData> LineDatas, bool Rebuild, const FVector2D ContentSize_, int32 ItemNum_, float ItemMaxValue, float ItemMinValue, TArray<FLineStyle> LineStyles_, ELineType LineType)
{

	bRebuild = Rebuild;

	if (bRebuild)
	{
		ContentSize.X = int32(ContentSize_.X);
		ContentSize.Y = int32(ContentSize_.Y);
		LineStyles.Empty();
		LineStyles = LineStyles_;
		ItemNum = ItemNum_;
		BGImageDatas.Empty();
	}


	CurveLineType = (LineType == ELineType::Linear) ? ERichCurveInterpMode::RCIM_Linear : ERichCurveInterpMode::RCIM_Cubic;


	if (BGIsSettings.Num() == 0)
	{
		for (FLineData data : LineDatas)
			BGIsSettings.Add(false);
	}


	LineDataPoints.Empty();

	for (int32 i = 0; i < LineDatas.Num() && i < LineStyles.Num(); i++)
	{
		TArray<float> ValueArray;

		LineDatas[i].LineData.GenerateValueArray(ValueArray);
		FCurvePointsArray Curve;
		for (int32 j = 0; j < ItemNum && j < ValueArray.Num(); j++)
		{
			float DataX, DataY;
			DataX = ceil(ContentSize.X / (ItemNum - 1)*j);
			DataY = ContentSize.Y*(ItemMaxValue - ValueArray[j]) / (ItemMaxValue - ItemMinValue);

			Curve.CurvePoints.Add(FVector2D(DataX, DataY));
		}
		LineDataPoints.Add(Curve);
	}

	if (!BGImageDatas.Num())
	{
		BGImageDatas.SetNum(LineDataPoints.Num());
	}

	AddCurveKey(LineDataPoints);

	for (int32 i = 0; i < LineStyles.Num() && i < LineDatas.Num(); i++)
	{
		if (LineStyles[i].DashLine)
		{
			MultiAllCurvePoints[i].DashLineList.Empty();
			for (int32 j = 0; j < MultiAllCurvePoints[i].CurvePoints.Num() - 2; j += 6)
			{
				FDashLine PreLine;

				PreLine.PreDashLine.Add(MultiAllCurvePoints[i].CurvePoints[j]);
				PreLine.PreDashLine.Add(MultiAllCurvePoints[i].CurvePoints[j + 1]);
				PreLine.PreDashLine.Add(MultiAllCurvePoints[i].CurvePoints[j + 2]);


				MultiAllCurvePoints[i].DashLineList.Add(PreLine);
			}
			//MultiAllCurvePoints[i].
		}
	}
	

	//bResetData = true;

	//bShowingEnd = false;

}


void UMultiCurveCanvasPanel::AddCurveKey(const TArray<FCurvePointsArray> LineDatas)
{
	//FloatCurves.Empty();
	MultiAllCurvePoints.Empty();
	for (int32 i = 0; i < LineStyles.Num() && i < LineDatas.Num(); i++)
	{
		FloatCurve->Reset();

		FCurvePointsArray Curve;
		MultiAllCurvePoints.Add(Curve);
		//FRichCurve* FloatCurve = new(FRichCurve);
		for (int32 j = 0; j < LineDatas[i].CurvePoints.Num(); j++)
		{
			FloatCurve->SetKeyInterpMode(FloatCurve->AddKey(LineDatas[i].CurvePoints[j].X, LineDatas[i].CurvePoints[j].Y), CurveLineType);	
		}
		//FloatCurves.Add(FloatCurve);
		for (int32 k = 0; k < LineDatas[i].CurvePoints.Num() - 1; k++)
		{
			TArray<FVector2D> CurvePoint;
			CreateLinesForSegment(LineDatas[i].CurvePoints[k], LineDatas[i].CurvePoints[k+1], CurvePoint);
			MultiAllCurvePoints[i].CurvePoints += CurvePoint;
		}
		GetBGImageTextureData(MultiAllCurvePoints[i].CurvePoints, i);

	}




}

void UMultiCurveCanvasPanel::CreateLinesForSegment(const FVector2D& Key1, const FVector2D& Key2, TArray<FVector2D>& Points) const
{

	const float StepSize = 1.0f;
	//clamp to screen to avoid massive slowdown when zoomed in
	float StartX = FMath::Max(Key1.X, 0.0f);
	float EndX = FMath::Min(Key2.X, ContentSize.X);
	for (; StartX < EndX; StartX += StepSize)
	{
		float CurveIn = StartX;
		float CurveOut = FloatCurve->Eval(CurveIn);
		Points.Add(FVector2D(CurveIn, CurveOut));
	}
}

void UMultiCurveCanvasPanel::GetBGImageTextureData(const TArray<FVector2D>& CurvePointList, int32 Index)
{
	if (LineStyles[Index].BackgroundImage == nullptr)
		return;

	float ImageWidth = LineStyles[Index].BackgroundImage->GetSizeX();
	float ImageHeight = LineStyles[Index].BackgroundImage->GetSizeY();

	if (!BGImageDatas[Index].ImageData.Num())
	{
		TArray<uint8> ImageData;

		int32 bufSize = ImageWidth * ImageHeight * 4;
		ImageData.SetNum(bufSize);


		void* TextureData_array = (LineStyles[Index].BackgroundImage->GetPlatformData()->Mips[0]).BulkData.Lock(LOCK_READ_ONLY);

		FMemory::Memcpy(ImageData.GetData(), TextureData_array, bufSize);

		LineStyles[Index].BackgroundImage->GetPlatformData()->Mips[0].BulkData.Unlock();

		int32 length = ContentSize.X * ContentSize.Y * 4;

		BGImageDatas[Index].ImageData.SetNum(length);

		int32 SrcX, SrcY;
		for (int32 Y = 0; Y < ContentSize.Y; Y++)
		{
			SrcY = Y * ImageHeight / ContentSize.Y;
			for (int32 X = 0; X < ContentSize.X; X++)
			{
				SrcX = X * ImageWidth / ContentSize.X;
				int32 Dst_Index = (Y * ContentSize.X + X) * 4;
				int32 Scr_Index = (SrcY * ImageWidth + SrcX) * 4;
				
				BGImageDatas[Index].ImageData[Dst_Index] = ImageData[Scr_Index];
				BGImageDatas[Index].ImageData[Dst_Index + 1] = ImageData[Scr_Index + 1];
				BGImageDatas[Index].ImageData[Dst_Index + 2] = ImageData[Scr_Index + 2];
				BGImageDatas[Index].ImageData[Dst_Index + 3] = ImageData[Scr_Index + 3];
			}
		}

	}
	
	TArray<uint8> CurrentBGImage = BGImageDatas[Index].ImageData;

	if (Texture_Copys.Num() == 0)
	{
		for (int32 i = 0; i < LineDataPoints.Num(); i++)
		{
			UTexture2D* texture = UTexture2D::CreateTransient(ContentSize.X, ContentSize.Y, PF_B8G8R8A8);
			Texture_Copys.Add(texture);
		}

	}
	void* TextureData_Copy = (Texture_Copys[Index]->GetPlatformData()->Mips[0]).BulkData.Lock(LOCK_READ_WRITE);
	
	for (int32 Y = 0; Y < ContentSize.Y; Y++)
	{
		for (int32 X = 0; X < ContentSize.X; X++)
		{

			if (X >= CurvePointList.Num() || Y < (ceil(CurvePointList[X].Y)))
			{
				int32 index = (Y * ContentSize.X + X) * 4 + 3;

				CurrentBGImage[index] = 0;
			}

		}
	}
	FMemory::Memcpy(TextureData_Copy, CurrentBGImage.GetData(), CurrentBGImage.Num());
	Texture_Copys[Index]->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture_Copys[Index]->UpdateResource();
	SetBGImage(Index);

}
bool UMultiCurveCanvasPanel::SetBGImage(int32 Index_)
{
	if (BGIsSettings[Index_] == true)
	{
		return true;
	}
	bool SetImage = false;

	UImage*ImageWidget = Cast<UImage>(GetChildAt(Index_));

	if (Texture_Copys[Index_] != nullptr && ImageWidget != nullptr)
	{
		ImageWidget->SetBrushFromTexture(Texture_Copys[Index_]);
		ImageWidget->SetDesiredSizeOverride(ContentSize);
		BGIsSettings[Index_] = true;
		SetImage = true;
	}
	return SetImage;
}

void UMultiCurveCanvasPanel::SetLineShoworHide(int32 Index_, bool Show)
{
	LineStyles[Index_].LineColor.A = Show ? 1 : 0;
}


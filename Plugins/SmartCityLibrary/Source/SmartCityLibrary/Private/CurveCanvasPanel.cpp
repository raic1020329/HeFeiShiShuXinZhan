#include "CurveCanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Rendering/DrawElements.h"


UCurveCanvasPanel::UCurveCanvasPanel(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	FloatCurve = new(FRichCurve);
	//CurvePointIndex = 0;
}
void UCurveCanvasPanel::DrawCurve(FPaintContext  Context)
{

	if (LineDataPoints.Num() > 0)
	{
		Context.MaxLayer++;
		//if (bResetData||!bShowingEnd)
		//{
			/*if (bResetData)
			{
				CurvePointIndex = 0;				
				AllCurvePoints.Empty();
				RebuildCurvePoints.Empty();				
				TArray<FVector2D> CurvePoint;

				for (int32 i = 0; i < LineDataPoints.Num() - 1; i++)
				{
					CreateLinesForSegment(Context, LineDataPoints[i], LineDataPoints[i + 1], CurvePoint);
					AllCurvePoints += CurvePoint;
					CurvePoint.Empty();
				}
				
				TargetPointStep = floor(AllCurvePoints.Num() / (ItemNum - 1));
				TargetPoint = TargetPointStep;
				if (!bRebuild)
				{
					TargetPoint = AllCurvePoints.Num();
					CurvePointIndex = floor(AllCurvePoints.Num() - AllCurvePoints.Num() / (ItemNum - 1)) - 1;
					for (int32 i = 0; i < CurvePointIndex; i++)
						RebuildCurvePoints.Add(AllCurvePoints[i]);
				}
					
				bResetData = false;
			}
				
			int32 PreAddPointNum = FMath::Clamp(AllCurvePoints.Num()* UGameplayStatics::GetWorldDeltaSeconds(this) / LineStyle.LineAnimTime, 1.f, 20.f);
			for (int32 i = 0; i < PreAddPointNum && CurvePointIndex < AllCurvePoints.Num(); i++)
			{
				RebuildCurvePoints.Add(AllCurvePoints[CurvePointIndex++]);
				if (CurvePointIndex >= TargetPoint)
				{
					TargetPoint += TargetPointStep;
					OnTimeCall.Broadcast();
				}

			}
			if (RebuildCurvePoints.Num() > 2)
			{
				FSlateDrawElement::MakeLines(
					Context.OutDrawElements,
					Context.MaxLayer,
					Context.AllottedGeometry.ToPaintGeometry(),
					RebuildCurvePoints,
					ESlateDrawEffect::None,
					LineStyle.LineColor,
					true,
					LineStyle.Thickness
				);
			}
			GetBGImageTextureData(RebuildCurvePoints);
			if (!(CurvePointIndex < AllCurvePoints.Num()))
			{
				CurvePointIndex = 0;
				RebuildCurvePoints.Empty();
				AllCurvePoints.RemoveAt(1);
				AllCurvePoints.RemoveAt(0);
				LineDataPoints[0].X += 2;
				bShowingEnd = true;
			}
			
			

		}*/
		//else
		//{
		if (LineStyle.DashLine && DashLineList.Num() > 0)
		{

			for (int32 i = 0; i < DashLineList.Num(); i++)
			{

				FSlateDrawElement::MakeLines(
					Context.OutDrawElements,
					Context.MaxLayer,
					Context.AllottedGeometry.ToPaintGeometry(),
					DashLineList[i].PreDashLine,
					ESlateDrawEffect::None,
					LineStyle.LineColor,
					true,
					LineStyle.Thickness
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
					AllCurvePoints,
					ESlateDrawEffect::None,
					LineStyle.LineColor,
					true,
					LineStyle.Thickness
				);
			}
			else
			{
				FSlateDrawElement::MakeLines(
					Context.OutDrawElements,
					Context.MaxLayer,
					Context.AllottedGeometry.ToPaintGeometry(),
					LineDataPoints,
					ESlateDrawEffect::None,
					LineStyle.LineColor,
					true,
					LineStyle.Thickness
				);
			}
		}
		
		//}
		
	}
	
}



void UCurveCanvasPanel::TurnDataToLineDataPoint(TMap<FString, float> LineData, bool Rebuild, const FVector2D ContentSize_, int32 ItemNum_, float ItemMaxValue, float ItemMinValue, FLineStyle LineStyle_, ELineType LineType)
{

	//bRebuild = Rebuild;	


	if (Rebuild)
	{
		ContentSize.X = int32(ContentSize_.X);
		ContentSize.Y = int32(ContentSize_.Y);
		LineStyle = LineStyle_;
		ItemNum = ItemNum_;
		//CurveStyle = InStyle;
		BGImageData.Empty();
	}


	CurveLineType = (LineType == ELineType::Linear) ? ERichCurveInterpMode::RCIM_Linear : ERichCurveInterpMode::RCIM_Cubic;


	TArray<float> ValueArray;

	LineData.GenerateValueArray(ValueArray);
	LineDataPoints.Empty();
	for (int32 i = 0; i < ItemNum && i < ValueArray.Num(); i++)
	{
		float DataX, DataY;
		DataX = ceil(ContentSize.X / (ItemNum - 1)*i);
		DataY = ContentSize.Y*(ItemMaxValue - ValueArray[i]) / (ItemMaxValue - ItemMinValue);

		LineDataPoints.Add(FVector2D(DataX, DataY));
	}

	AddCurveKey(LineDataPoints);



	if (LineStyle.DashLine)
	{
		DashLineList.Empty();
		for (int32 i = 0; i < AllCurvePoints.Num() - 2; i += 6)
		{
			FDashLine PreLine;

			PreLine.PreDashLine.Add(AllCurvePoints[i]);
			PreLine.PreDashLine.Add(AllCurvePoints[i + 1]);
			PreLine.PreDashLine.Add(AllCurvePoints[i + 2]);


			DashLineList.Add(PreLine);
		}

	}


	//bResetData = true;
	//bShowingEnd = false;

}


//void UCurveCanvasPanel::TurnDataToLineDataPoint(TMap<FString, float> LineData, bool Rebuild, FCurveMapStyle InStyle/*const FVector2D ContentSize_, int32 ItemNum_, float ItemMaxValue, float ItemMinValue, FLineStyle LineStyle_, ELineType LineType, bool DashLine*/)
//void TurnDataToLineDataPoint(TMap<FString, float> LineData, bool Rebuild, const FVector2D ContentSize_, int32 ItemNum_, float ItemMaxValue, float ItemMinValue, FLineStyle LineStyle_, ELineType LineType)



void UCurveCanvasPanel::AddCurveKey(const TArray<FVector2D> LineData)
{
	FloatCurve->Reset();
	AllCurvePoints.Empty();
	
	for (int32 i = 0; i < LineData.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Time:  %f,   Value:  %f"), LineData[], key.Value);
		FloatCurve->SetKeyInterpMode(FloatCurve->AddKey(LineData[i].X, LineData[i].Y), CurveLineType);
	}

	for (int32 i = 0; i < LineData.Num() - 1; i++)
	{
		TArray<FVector2D> CurvePoint;
		CreateLinesForSegment(LineData[i], LineData[i + 1], CurvePoint);
		AllCurvePoints += CurvePoint;
		//CurvePoint.Empty();
	}

	GetBGImageTextureData(AllCurvePoints);
}

void UCurveCanvasPanel::CreateLinesForSegment(const FVector2D& Key1, const FVector2D& Key2, TArray<FVector2D>& Points) const
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
void UCurveCanvasPanel::GetBGImageTextureData(const TArray<FVector2D>& CurvePointList)
{
	if (LineStyle.BackgroundImage == nullptr)
		return;

	float ImageWidth = LineStyle.BackgroundImage->GetSizeX();
	float ImageHeight = LineStyle.BackgroundImage->GetSizeY();

	if (!BGImageData.Num())
	{
		//LineStyle.BackgroundImage->Source.GetMipData(BGImageData, 0);
		TArray<uint8> ImageData;
		
		int32 bufSize = ImageWidth * ImageHeight * 4;
		
		ImageData.SetNum(bufSize);

		void* TextureData_array = (LineStyle.BackgroundImage->GetPlatformData()->Mips[0]).BulkData.Lock(LOCK_READ_ONLY);

		FMemory::Memcpy(ImageData.GetData(), TextureData_array, bufSize);
		LineStyle.BackgroundImage->GetPlatformData()->Mips[0].BulkData.Unlock();
		int32 length = ContentSize.X * ContentSize.Y * 4;

		BGImageData.SetNum(length);
		int32 SrcX, SrcY;
		for (int32 Y = 0; Y < ContentSize.Y; Y++)
		{
			SrcY = Y * ImageHeight / ContentSize.Y;
			for (int32 X = 0; X < ContentSize.X; X++)
			{
				SrcX = X * ImageWidth / ContentSize.X;
				int32 Dst_Index = (Y * ContentSize.X + X) * 4;
				int32 Scr_Index = (SrcY * ImageWidth + SrcX) * 4;
				BGImageData[Dst_Index] = ImageData[Scr_Index];
				BGImageData[Dst_Index + 1] = ImageData[Scr_Index + 1];
				BGImageData[Dst_Index + 2] = ImageData[Scr_Index + 2];
				BGImageData[Dst_Index + 3] = ImageData[Scr_Index + 3];
			}
		}

		

	}
	
	TArray<uint8> CurrentBGImage = BGImageData;
	if (Texture_Copy == nullptr)
	{
		Texture_Copy = UTexture2D::CreateTransient(ContentSize.X, ContentSize.Y, PF_B8G8R8A8);	
	}

	void* TextureData_Copy = (Texture_Copy->GetPlatformData()->Mips[0]).BulkData.Lock(LOCK_READ_WRITE);

	for (int32 Y = 0; Y < ContentSize.Y; Y++)
	{
		for (int32 X = 0; X < ContentSize.X ; X++)
		{

			if (X >= CurvePointList.Num() || Y < (ceil(CurvePointList[X].Y)))
			{
				int32 Index = (Y * ContentSize.X + X) * 4 + 3;
				CurrentBGImage[Index] = 0;
			}
		}
	}
	FMemory::Memcpy(TextureData_Copy, CurrentBGImage.GetData(), CurrentBGImage.Num());
	Texture_Copy->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture_Copy->UpdateResource();
	SetBGImage();

}
bool UCurveCanvasPanel::SetBGImage()
{
	if (BGIsSetting == true)
	{
		return true;
	}
	bool SetImage = false;
	UImage*ImageWidget = Cast<UImage>(GetChildAt(0));
	if (Texture_Copy != nullptr && ImageWidget != nullptr)
	{
		ImageWidget->SetBrushFromTexture(Texture_Copy);
		ImageWidget->SetDesiredSizeOverride(ContentSize);
		BGIsSetting = true;
		SetImage = true;
	}
	return SetImage;
}


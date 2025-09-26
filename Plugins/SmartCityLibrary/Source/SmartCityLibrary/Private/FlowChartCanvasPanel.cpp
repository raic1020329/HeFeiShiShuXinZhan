#include "FlowChartCanvasPanel.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"


//#include "Runtime/AppFramework/Private/Widgets/Testing/STestSuite.cpp"


UFlowChartCanvasPanel::UFlowChartCanvasPanel(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	FloatCurve = new(FRichCurve);
}

TArray<FFlowButtonInfo> UFlowChartCanvasPanel::TurnDataToLineDataPoint(TArray<FFlowChartData> FlowChartData, FLinearColor InColor, const FVector2D InContentSize, float ButtonOffsets, float Interval, int32 ShowIndex)
{

	TArray<FFlowButtonInfo> ButtonList;

	if (FlowChartData.Num() < 1)
		return ButtonList;

	ContentSize = InContentSize;
	SubFlowLines.Empty();
	SubLevelSums.Empty();
	MainFlowLines.Empty();

	FlowColor = InColor;
	
	int32 LevelNum = 0;

	float Sum = 0;
	for (int32 i = 0; i < FlowChartData.Num(); i++)
	{
		if (LevelNum < FlowChartData[i].Level)
			LevelNum = FlowChartData[i].Level;

		if (FlowChartData[i].Level == 1)
			Sum += FlowChartData[i].Value;
	}


	int32 SubIndex = 0;
	for (int32 i = 0; i < FlowChartData.Num(); i++)
	{		
		if (FlowChartData[i].Level == LevelNum - 1)
		{
			FString Name = FlowChartData[i].Target;
			float Value = FlowChartData[i].Value;
			FSubInfo sub;
			sub.SourceName = Name;
			sub.SubValue = Value;
			SubLevelSums.Add(SubIndex, sub);
			SubIndex++;
		}
	}

	if (Sum > 0)
	{
		int32 LevelIndex = 0;
		FString SourceName;
		FString TargetName;
		int32 PreLevelItemIndex = 0;
		int32 MainFlowIndex = 0;
		int32 SubFlowIndex = 0;
		float SubOffset = 0;
		float SubLevelPreOffset_Start = 0;
		float SubLevelPreOffset_End = 0;

		float PreLevelStep = ContentSize.X / LevelNum;
		

		for (int32 i = 0; i < FlowChartData.Num(); i++)
		{	

			bool FirstFlow = false;

			float FlowThickness = FlowChartData[i].Value / Sum * ContentSize.Y;;
			if (LevelIndex != FlowChartData[i].Level)
			{
				LevelIndex = FlowChartData[i].Level;
				PreLevelItemIndex = 0;
			}

			if (LevelNum != FlowChartData[i].Level)
			{
				FVector2D P1;
				FVector2D P2;
				FVector2D P3;
				FVector2D P4;
				FVector2D LastP3;
				FVector2D LastP4;
				if (PreLevelItemIndex == 0)
				{

					FirstFlow = true;
					SourceName = FlowChartData[i].Source;
					TargetName = FlowChartData[i].Target;

					FFlowButtonInfo FlowButton;					
					FlowButton.ButtonPosition = FVector2D((LevelIndex - 1)*PreLevelStep, 0);
					FlowButton.ButtonLength = FlowThickness;
					FlowButton.ButtonValue = FlowChartData[i].Value;
					FlowButton.ButtonName = SourceName;
					ButtonList.Add(FlowButton);
					
					P1 = FVector2D((LevelIndex - 1)*PreLevelStep + ButtonOffsets, 0);
					P2 = FVector2D(LevelIndex*PreLevelStep, 0);
				}
				else
				{	
					if (SourceName == FlowChartData[i].Source)
					{
						FFlowButtonInfo*LastButton = &ButtonList.Last();
						LastButton->ButtonLength += FlowThickness;
						LastButton->ButtonValue += FlowChartData[i].Value;

						P1 = LastP3;
					}
					else
					{
						FirstFlow = true;
						SourceName = FlowChartData[i].Source;

						FFlowButtonInfo FlowButton;
						FFlowButtonInfo LastFlowButton = ButtonList.Last();
						FlowButton.ButtonPosition = FVector2D(LastFlowButton.ButtonPosition.X, LastFlowButton.ButtonPosition.Y + LastFlowButton.ButtonLength + Interval);
						FlowButton.ButtonLength = FlowThickness;
						FlowButton.ButtonValue = FlowChartData[i].Value;
						FlowButton.ButtonName = SourceName;
						ButtonList.Add(FlowButton);
						
						P1 = FVector2D(LastP3.X, LastP3.Y + Interval);
					}
					if (TargetName == FlowChartData[i].Target)
					{
						P2 = LastP4;
					}
					else
					{
						TargetName = FlowChartData[i].Target;
						P2 = FVector2D(LastP4.X, LastP4.Y + Interval);
					}
				}
				P3 = FVector2D(P1.X, P1.Y + FlowThickness);
				P4 = FVector2D(P2.X, P2.Y + FlowThickness);
				LastP3 = P3;
				LastP4 = P4;
				TArray<FFlowLine> FlowLines;
				AddSubFlowCurveKey(P1, P2, P3, P4, FlowLines, FirstFlow);
				MainFlowLines.Add(MainFlowIndex,FlowLines);
				MainFlowIndex++;
				

			}
			else
			{			
				if (!SubLevelSums.Find(ShowIndex))
					return ButtonList;
				if (PreLevelItemIndex == 0)
				{
					FirstFlow = true;
					for (int32 k = 0; k < SubIndex; k++)
					{
						float SubLength = SubLevelSums.Find(k)->SubValue / Sum * ContentSize.Y;
						FFlowButtonInfo FlowButton;
						FlowButton.ButtonEnable = true;
						FlowButton.ButtonIndex = k;
						FlowButton.ButtonLength = SubLength;				
						FlowButton.ButtonValue = SubLevelSums.Find(k)->SubValue;
						FlowButton.ButtonName = SubLevelSums.Find(k)->SourceName;
						float LastLength = k == 0 ? 0 : ButtonList.Last().ButtonPosition.Y + ButtonList.Last().ButtonLength + Interval;
						FlowButton.ButtonPosition = FVector2D((LevelIndex - 1)*PreLevelStep, LastLength);
						ButtonList.Add(FlowButton);
						if (k < ShowIndex)
						{
							
							SubOffset += (SubLength + Interval);
						}
					}
					//UE_LOG(LogTemp, Log, TEXT("SubOffset:%f"), SubOffset);
				}

				FString Name = SubLevelSums.Find(ShowIndex)->SourceName;
				if (FlowChartData[i].Source != Name)
				{
					PreLevelItemIndex++;
					continue;
				}

				const float SubSum = SubLevelSums.Find(ShowIndex)->SubValue;
				float TargetLength = FlowChartData[i].Value / SubSum * ContentSize.Y;

				FVector2D P1 = FVector2D((LevelIndex - 1)*PreLevelStep + ButtonOffsets, SubOffset + SubLevelPreOffset_Start);
				FVector2D P2 = FVector2D(LevelIndex*PreLevelStep, SubLevelPreOffset_End);
				FVector2D P3 = FVector2D(P1.X, P1.Y + FlowThickness);
				FVector2D P4 = FVector2D(P2.X, P2.Y + TargetLength);
				//UE_LOG(LogTemp, Log, TEXT("SubOffset:%f,SubLevelPreOffset_Start:%f"), SubOffset, SubLevelPreOffset_Start);
				FFlowButtonInfo FlowButton;
				FlowButton.ButtonPosition = P2;
				FlowButton.ButtonLength = P4.Y - P2.Y;
				FlowButton.FinalButton = true;
				FlowButton.ButtonValue = FlowChartData[i].Value;
				FlowButton.ButtonName = FlowChartData[i].Target;
				ButtonList.Add(FlowButton);

				SubLevelPreOffset_Start = P3.Y - SubOffset;
				SubLevelPreOffset_End = P4.Y + Interval;

				TArray<FFlowLine> FlowLines;
				AddSubFlowCurveKey(P1, P2, P3, P4, FlowLines, FirstFlow);
				SubFlowLines.Add(SubFlowIndex, FlowLines);


				SubFlowIndex++;

			
			}

			
			PreLevelItemIndex++;
		}
	}

	return ButtonList;
}

void UFlowChartCanvasPanel::DrawFlow(FPaintContext Context)
{

		Context.MaxLayer++;

		for (int32 i = 0; i < MainFlowLines.Num(); i++)
		{
			TArray<FFlowLine> *Line = MainFlowLines.Find(i);
			for (int32 j = 0; j < Line->Num(); j++)
			{
				TArray<FVector2D> LinePoints;
				LinePoints.Add((*Line)[j].PointUp);
				LinePoints.Add((*Line)[j].PointDown);

				FSlateDrawElement::MakeLines(
					Context.OutDrawElements,
					Context.MaxLayer,
					Context.AllottedGeometry.ToPaintGeometry(),
					LinePoints,
					ESlateDrawEffect::None,
					FlowColor,
					true,
					1.0

				);
			}
		}

		
		for (int32 i = 0; i < SubFlowLines.Num(); i++)
		{
			TArray<FFlowLine> *Line = SubFlowLines.Find(i);

			for (int32 j = 0; j < Line->Num(); j++)
			{
				TArray<FVector2D> LinePoints;
				LinePoints.Add((*Line)[j].PointUp);
				LinePoints.Add((*Line)[j].PointDown);

				FSlateDrawElement::MakeLines(
					Context.OutDrawElements,
					Context.MaxLayer,
					Context.AllottedGeometry.ToPaintGeometry(),
					LinePoints,
					ESlateDrawEffect::None,
					FlowColor,
					true,
					1.f
				);
			}

			
		}


}

void UFlowChartCanvasPanel::AddSubFlowCurveKey(const FVector2D P1, const FVector2D P2, const FVector2D P3, const FVector2D P4, TArray<FFlowLine>& InLines, bool FirstIndex)
{
	FloatCurve->Reset();

	FloatCurve->SetKeyInterpMode(FloatCurve->AddKey(P1.X, P1.Y), ERichCurveInterpMode::RCIM_Cubic);
	FloatCurve->SetKeyInterpMode(FloatCurve->AddKey(P2.X, P2.Y), ERichCurveInterpMode::RCIM_Cubic);

	const float StepSize = 1.0f;
	//clamp to screen to avoid massive slowdown when zoomed in
	float StartX = FMath::Max(P1.X, 0.0f);
	float EndX = FMath::Min(P2.X, ContentSize.X);
	TArray<FVector2D> PointsA;
	int32 Offsets = FirstIndex ? 2 : 0;
	for (; StartX < EndX; StartX += StepSize)
	{
		float CurveIn = StartX;
		float CurveOut = FloatCurve->Eval(CurveIn) + Offsets;
		PointsA.Add(FVector2D(CurveIn, CurveOut));
	}

	FloatCurve->Reset();
	FloatCurve->SetKeyInterpMode(FloatCurve->AddKey(P3.X, P3.Y), ERichCurveInterpMode::RCIM_Cubic);
	FloatCurve->SetKeyInterpMode(FloatCurve->AddKey(P4.X, P4.Y), ERichCurveInterpMode::RCIM_Cubic);

	StartX = FMath::Max(P3.X, 0.0f);
	EndX = FMath::Min(P4.X, ContentSize.X);
	TArray<FVector2D> PointsB;
	for (; StartX < EndX; StartX += StepSize)
	{
		float CurveIn = StartX;
		float CurveOut = FloatCurve->Eval(CurveIn) - 2;
		PointsB.Add(FVector2D(CurveIn, CurveOut));
	}

	for (int32 i = 0; i < PointsA.Num() && i < PointsB.Num(); i++)
	{
		FFlowLine Lines;
		Lines.PointUp = PointsA[i];
		Lines.PointDown = PointsB[i];

		InLines.Add(Lines);
	}



}


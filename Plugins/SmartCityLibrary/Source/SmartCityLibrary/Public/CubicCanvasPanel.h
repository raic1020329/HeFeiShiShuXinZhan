#pragma once
#include "Components/CanvasPanel.h"
#include "CubicCanvasPanel.generated.h"


USTRUCT()
struct FCubicPoint
{
	GENERATED_USTRUCT_BODY()
	TArray<FVector2D> CubicDataPoints;
};

UCLASS()
class UCubicCanvasPanel : public UCanvasPanel
{
	GENERATED_UCLASS_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "CubicCanvasPanel")
		TArray<float> TurnDataToLineDataPoint(TMap<FString, float> LineData, TArray<FLinearColor> InColorList, const FVector2D ContentSize, float Thickness = 1.f, float PointDensity = 10.f);



	UFUNCTION(BlueprintCallable, Category = "CubicCanvasPanel")
		void DrawCubic(FPaintContext Context);

	UFUNCTION(BlueprintPure, Category = "CubicCanvasPanel")
		TArray<FVector2D>GetCubicCenterPoints();

	void BubbleAlgorithm(TArray<float>& inArray, TArray<FString>& KeyArray_);

	UFUNCTION(BlueprintPure, Category = "CubicCanvasPanel")
	TArray<float> BubbleAlgorithm_float(TArray<float> inArray);

	void CalcSectionRatio(TArray<float>& inArray, TArray<float>& OutArray);
private:
	TArray<FCubicPoint> AllCubicPoints;

	TArray<FVector2D> CubicCenterPoints;
	TArray<FVector2D> PointsList;

	float CubicThickness;

	TArray<FString> Keys;
	TMap<FString, FLinearColor> ColorMap;

	float Step_X;
	float Step_Y;
};
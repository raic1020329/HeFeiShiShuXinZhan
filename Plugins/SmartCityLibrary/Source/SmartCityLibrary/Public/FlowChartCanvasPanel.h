#pragma once
#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Brushes/SlateColorBrush.h"

#include "Styling/CoreStyle.h"
#include "FlowChartCanvasPanel.generated.h"


USTRUCT(BlueprintType)
struct FFlowChartData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		int32 Level = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		FString Source;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		FString Target;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		float Value = 0;
};


USTRUCT()
struct FFlowLine
{
	GENERATED_USTRUCT_BODY()

	FVector2D PointUp;
	FVector2D PointDown;

};

USTRUCT()
struct FSubInfo
{
	GENERATED_USTRUCT_BODY()
	FString SourceName;

	float SubValue;
};

USTRUCT(BlueprintType)
struct FFlowButtonInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		FVector2D ButtonPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		float ButtonLength = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		bool ButtonEnable = false;

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		int32 ButtonIndex = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		bool FinalButton = false;

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		float ButtonValue = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		FString ButtonName;
};

UCLASS()
class UFlowChartCanvasPanel : public UCanvasPanel
{
	GENERATED_UCLASS_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "FlowChartCanvasPanel")
		TArray<FFlowButtonInfo> TurnDataToLineDataPoint(TArray<FFlowChartData> FlowData, FLinearColor InColor, const FVector2D InContentSize, float ButtonOffsets, float Interval = 10.f, int32 ShowIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "FlowChartCanvasPanel")
	void DrawFlow(FPaintContext Context);

	void AddSubFlowCurveKey(const FVector2D P1, const FVector2D P2, const FVector2D P3, const FVector2D P4, TArray<FFlowLine>& InLines, bool FirstIndex);
	
private:

	FRichCurve *FloatCurve = NULL;
	FLinearColor FlowColor;
	
	FVector2D ContentSize;
	TMap<int32, FSubInfo> SubLevelSums;
	TMap<int32, TArray<FFlowLine>> MainFlowLines;
	TMap<int32,TArray<FFlowLine>> SubFlowLines;


};



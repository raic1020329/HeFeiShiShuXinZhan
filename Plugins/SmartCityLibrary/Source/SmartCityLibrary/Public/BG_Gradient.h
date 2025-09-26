#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "BG_Gradient.generated.h"


USTRUCT()
struct FDashedData
{
	GENERATED_USTRUCT_BODY()

	FVector2D StartPoint;
	FVector2D EndPoint;
	FLinearColor LineColor;
	float Thickness;

};

USTRUCT(BlueprintType)
struct FDashedLineStyle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool CreateDashLine = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bBeUsedHorizontal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FLinearColor DashLineColor = FLinearColor::Gray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float DashLineThickness = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 DashDensity = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float DashRatio = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool CreateAssiztLine = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool ShowHorizontal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool ShowVertical = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool CreateAverageLine = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FLinearColor AverageLineColor = FLinearColor::Red;

};

UCLASS()
class UBG_Gradient : public UCanvasPanel
{
	GENERATED_UCLASS_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "BG_Gradient")
		void SetBGStyle(const FVector2D InContentSize, FDashedLineStyle InDashLineStyle, int32 SectionNum_X = 5, int32 SectionNum_Y = 3);
	
	UFUNCTION(BlueprintCallable, Category = "BG_Gradient")
		void SetAverageValue(float AvgValue);

	UFUNCTION(BlueprintCallable, Category = "BG_Gradient")
		void DrawBG(FPaintContext Context);

	FVector2D ContentSize;

	TArray<FDashedData> DashedLineList;

	FDashedLineStyle DashLineStyle;

	TArray<FDashedData> AvgLineList;

};

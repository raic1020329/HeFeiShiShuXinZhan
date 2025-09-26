#pragma once

#include "Components/CanvasPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "CurveCanvasPanel.generated.h"




UENUM(BlueprintType)
enum  ELineType
{
	Linear UMETA(DisplayName = "Linear"),
	Auto UMETA(DisplayName = "Auto")
 };

USTRUCT()
struct FDashLine
{
	GENERATED_USTRUCT_BODY()
		TArray<FVector2D> PreDashLine;
	
};


USTRUCT(BlueprintType)
struct FLineStyle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
	UTexture2D* BackgroundImage = nullptr;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		float Thickness = 1.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		FLinearColor LineColor = FLinearColor::White;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		float LineAnimTime = 1.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		bool DashLine = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
		FText Category;

};

//USTRUCT(BlueprintType)
//struct FCurvePanelStyle
//{
//	GENERATED_USTRUCT_BODY()
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//	FVector2D ContentSize;
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//	int32 ItemNum;
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//	float ItemMaxValue;
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//	float ItemMinValue;
//	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//	//FLineStyle LineStyle;
//	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//	//ELineType LineType;
//	
//};






UCLASS()
class UCurveCanvasPanel : public UCanvasPanel
{
	GENERATED_UCLASS_BODY()


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCall);
	UPROPERTY(BlueprintAssignable)
		FCall OnTimeCall;

	UFUNCTION(BlueprintCallable, Category = "CurveCanvasPanel")
		void DrawCurve(FPaintContext Context);

	void CreateLinesForSegment(const FVector2D& Key1, const FVector2D& Key2, TArray<FVector2D>& Points) const;

	UFUNCTION(BlueprintCallable, Category = "CurveCanvasPanel")
	void TurnDataToLineDataPoint(TMap<FString, float> LineData, bool Rebuild, const FVector2D ContentSize_, int32 ItemNum_, float ItemMaxValue, float ItemMinValue, FLineStyle LineStyle_, ELineType LineType = ELineType::Linear);
		

	void AddCurveKey(const TArray<FVector2D> LineData);

	void GetBGImageTextureData(const TArray<FVector2D>& CurvePointList);

	bool SetBGImage();

private:
	TArray<FVector2D> LineDataPoints;

	FRichCurve *FloatCurve = NULL;



	TArray<FVector2D> AllCurvePoints;

	TArray<uint8> BGImageData;



	ERichCurveInterpMode CurveLineType;

	UTexture2D* Texture_Copy = nullptr;

	bool BGIsSetting = false;

	//FCurvePanelStyle CurveStyle;
	int32 ItemNum;
	FLineStyle LineStyle;


	FVector2D ContentSize;

	TArray<FDashLine> DashLineList;


};

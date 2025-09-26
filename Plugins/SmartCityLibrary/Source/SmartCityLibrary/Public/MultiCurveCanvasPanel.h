#pragma once
#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "CurveCanvasPanel.h"
#include "MultiCurveCanvasPanel.generated.h"




//UENUM()
//enum EMultiLineType
//{
//	MultiLinear UMETA(DisplayName = "Linear"),
//	MultiAuto UMETA(DisplayName = "Auto"),
// };

//USTRUCT(BlueprintType)
//struct FMultiLineStyle
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//		UTexture2D* BackgroundImage = nullptr;
//
//
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//		float Thickness = 1.f;
//
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//		FLinearColor LineColor = FLinearColor::White;
//
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//		float LineAnimTime = 1.f;
//
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Settings")
//		FString Category;
//};

USTRUCT()
struct FCurvePointsArray
{
	GENERATED_USTRUCT_BODY()

	TArray<FVector2D> CurvePoints;

	TArray<FDashLine> DashLineList;
	//TArray<uint8> BGImageData;
};


USTRUCT()
struct FBGImageDatas
{
	GENERATED_USTRUCT_BODY()

	TArray<uint8> ImageData;
};


USTRUCT(BlueprintType)
struct FLineData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
		TMap<FString, float> LineData;
};

UCLASS()
class UMultiCurveCanvasPanel : public UCanvasPanel
{
	GENERATED_UCLASS_BODY()


public:
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMultiCurveCall);

	UPROPERTY(BlueprintAssignable)
		FMultiCurveCall OnTimeCall;
	
	UFUNCTION(BlueprintCallable, Category = "MultiCurveCanvasPanel")
		void DrawCurve(FPaintContext Context);
	
	void CreateLinesForSegment(const FVector2D& Key1, const FVector2D& Key2, TArray<FVector2D>& Points) const;
	
	UFUNCTION(BlueprintCallable, Category = "MultiCurveCanvasPanel")
		void TurnDataToLineDataPoint(TArray<FLineData> LineDatas, bool Rebuild, const FVector2D ContentSize_, int32 ItemNum_, float ItemMaxValue, float ItemMinValue, TArray<FLineStyle> LineStyles_, ELineType LineType = ELineType::Linear);
	
	void AddCurveKey(const TArray<FCurvePointsArray> LineDatas);

	void GetBGImageTextureData(const TArray<FVector2D>& CurvePointList, int32 Index);
	
	bool SetBGImage(int32 Index_);

	UFUNCTION(BlueprintCallable, Category = "MultiCurveCanvasPanel")
	void SetLineShoworHide(int32 Index_, bool Show = true);

private:
	TArray<FCurvePointsArray> LineDataPoints;

	//TArray<FRichCurve*> FloatCurves;
	FRichCurve *FloatCurve = NULL;
	bool bRebuild = true;
	//bool bResetData = false;

	//TArray<FCurvePointsArray> MultiRebuildCurvePoints;

	TArray<FCurvePointsArray> MultiAllCurvePoints;

	//TArray<uint8> BGImageData;

	FVector2D ContentSize;

	//int32 CurvePointIndex;


	TArray<UTexture2D*> Texture_Copys;

	ERichCurveInterpMode CurveLineType;

	TArray<bool> BGIsSettings;

	//bool bShowingEnd = true;

	int32 ItemNum;

	TArray<FLineStyle> LineStyles;
	TArray<FBGImageDatas> BGImageDatas;

	//int32 TargetPointStep;

	//int32 TargetPoint;

};

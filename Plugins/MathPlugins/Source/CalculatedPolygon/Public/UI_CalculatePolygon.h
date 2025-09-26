// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Async/AsyncWork.h"
#include "UI_CalculatePolygon.generated.h"


class FOpenCollisionAnsyTask :public FNonAbandonableTask
{
public:

	FOpenCollisionAnsyTask(TFunction<void()>& InFuntion)
		:Funtion(InFuntion)
	{
	}
	void DoWork()
	{
		Funtion();
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FOpenCollisionAnsyTask, STATGROUP_ThreadPoolAsyncTasks);
	}
private:
	TFunction<void()> Funtion;
};


UENUM(BlueprintType)
enum class E_CalculateType :uint8 {
	eNone,
	//水平距离
	eHorizontal,
	//垂直距离
	eVertical,
	//空间距离
	eVector,
	//角度
	eAngle,
	//面积
	eArea
};

/**
 * 
 */
UCLASS()
class CALCULATEDPOLYGON_API UUI_CalculatePolygon : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void NativePreConstruct();
	virtual void NativeConstruct() override;

	//virtual void NativeDestruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;


	FString GetText(const E_CalculateType& InType,const FVector& InStartPos, const FVector& InEndPos) const;

	UFUNCTION(BlueprintCallable)
	void ClearData();

	void CreateUI();

	UFUNCTION(BlueprintNativeEvent)
		void CreateResult();

	void CreateLength();
	void CreateAngel();
	void CreateArea();

	UFUNCTION(BlueprintCallable)
	void OpenMeshCollision();
	UFUNCTION(BlueprintCallable)
	void CloseMeshCollision();

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> m_WorldPos;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> m_ScreenPos;
	UPROPERTY(BlueprintReadWrite)
	E_CalculateType m_CalculateType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bLineStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_ResetArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor m_LineColor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUI_CalculateResult> MyWidget;
	UPROPERTY(BlueprintReadWrite)
	TArray<class UUI_CalculateResult*> m_CalculateResult;


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MeshIsOpenCollision = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AStaticMeshActor*> OpenCollisionMeshArray;
};

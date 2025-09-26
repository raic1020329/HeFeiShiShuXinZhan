// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include <UI_CalculatePolygon.h>
#include "UI_CalculateResult.generated.h"


/**
 * 
 */
UCLASS()
class CALCULATEDPOLYGON_API UUI_CalculateResult : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//virtual void NativeConstruct() override;
	virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	//virtual void NativeDestruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	void SetValue(const FText& InValue) {
		m_Result = InValue;
		MyText->SetText(m_Result);
	}

	void SetValue(const TArray<FVector>& InPosArray, E_CalculateType InType = E_CalculateType::eNone) {
		m_WorldPosArray = InPosArray;
		m_CalculateType = InType;
	}
	
private:
	void LengthPoint();
	void AnglePoint();
	void AreaPoint();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText m_Result;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* MyText;

	TArray<FVector> m_WorldPosArray;
	FVector m_EndPos;

	E_CalculateType m_CalculateType;
};

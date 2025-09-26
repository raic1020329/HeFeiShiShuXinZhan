// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCommon.generated.h"

/**
 *
 */
class UCanvasPanel;
class UTextBlock;
UCLASS()
class DTFRAMETEMPLETE_API UCommon : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;


	UFUNCTION(BlueprintCallable)
	void initText(UTextBlock* _Textblock, const FText& _Text, const FSlateColor& _Color, const FSlateFontInfo& _Font);
	UFUNCTION(BlueprintCallable)
	void initProperty();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void UpdateData(const FText& _Text);
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* RootCanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlock_Key;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlock_Value;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlock_Unit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Key")	FText KeyText = FText::FromString("Key");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Key")	FSlateColor KeyColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Key")	FSlateFontInfo KeyFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Value")	FText ValueText = FText::FromString("Value");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Value")	FSlateColor ValueColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Value")	FSlateFontInfo ValueFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Unit")	FText UnitText = FText::FromString("Unit");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Unit")	FSlateColor UnitColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Unit")	FSlateFontInfo UnitFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")	bool bShowUnit = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting") FMargin UnitPadding;
};

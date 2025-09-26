#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "ButtonBase.h"
#include "CustomLayoutButton.generated.h"

UCLASS()
class DTFRAMETEMPLETE_API UCustomLayoutButton :public UCanvasPanel
{
	GENERATED_UCLASS_BODY()
public:
	//按钮交互类型 分别为复选按钮，互斥按钮，自由按钮
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EDTButtonOnClickType ButtonOnClickType;
	UPROPERTY(BlueprintAssignable, Category = "Setting")
	FLayoutButtonDelegate LayoutButtonDelegate;
	UFUNCTION(BlueprintCallable)
	void OnButtonClick(int32 Index);
public:
#if WITH_EDITOR
	// UWidget interface
	virtual const FText GetPaletteCategory() override;
	// End UWidget interface
#endif

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	// End UPanelWidget

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;


protected:
	UFUNCTION()
		void ButtonLeafCallBack(int32 Index);
protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};

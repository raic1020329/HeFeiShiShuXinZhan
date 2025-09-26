#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ButtonBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLayoutButtonDelegate, int32, ButtonIndex, bool, IsSelect);

UENUM(BlueprintType)
enum class EDTButtonOnClickType :uint8
{
	//互斥按钮
	MutexButton,
	//复选按钮
	CheckButton,
	//普通按钮
	FreeButton,
};

UENUM(BlueprintType)
enum class EDTButtonStatue :uint8
{
	Common,
	Select,
	Hovered,
};

USTRUCT(BlueprintType)
struct DTFRAMETEMPLETE_API FButtonLeafStyle
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		FSlateBrush Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		FSlateBrush Select;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		FSlateBrush Hovered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		FSlateBrush Pressed;
};

UCLASS(Abstract, BlueprintType, Blueprintable)
class DTFRAMETEMPLETE_API UDTButtonBase :public UUserWidget
{
	GENERATED_UCLASS_BODY()
public:
	DECLARE_DELEGATE_OneParam(FButtonOnClick, int32)

public:
	virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

public:
	UFUNCTION(BlueprintCallable)
		void ResertButton(EDTButtonOnClickType InType, bool bIsSelect);

public:
	FButtonOnClick ButtonOnClikDelegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Setting")
		int32 ButtonIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true", ExposeOnSpawn = true, InstanceEditable = true), Category = "Setting")
		FText ButtonName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Setting")
		FButtonLeafStyle ButtonStyle;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DTButtonBase")
		UButton* MyButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DTButtonBase")
		UTextBlock* MyText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "DTButtonBase")
		bool IsSelect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "DTButtonBase")
		EDTButtonStatue ButtonStatue;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void ButtonCommonStyle();
	UFUNCTION(BlueprintImplementableEvent)
		void ButtonSelectStyle();
	UFUNCTION(BlueprintImplementableEvent)
		void ButtonHoveredStyle();
	UFUNCTION(BlueprintPure)
		EDTButtonStatue GetButtonStuate() const;

private:
	UFUNCTION()
		void OnClickedCallBack();
	UFUNCTION()
		void OnHoveredCallBack();
	UFUNCTION()
		void OnUnHoveredCallBack();
	UFUNCTION()
		void OnPressCallBack();
protected:
	EDTButtonOnClickType OnClickType;
};

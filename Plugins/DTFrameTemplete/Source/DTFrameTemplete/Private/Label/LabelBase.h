#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabelBase.generated.h"

class UButton;
UCLASS(abstract)
class DTFRAMETEMPLETE_API ULabelBase :public UUserWidget
{
	GENERATED_UCLASS_BODY()
public:
	DECLARE_DELEGATE_OneParam(FLabelBaseDelegate, ULabelBase*);
public:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* MyButton;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Label Base")
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Label Base")
	bool bIsOpenDetailPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Label Base")
	bool bIsShowUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Label Base")
	FString LabelType;
	FLabelBaseDelegate LabelBaseDelegate;

	UFUNCTION(BlueprintCallable,Category="Label Base")
	void UpdateLocation(const FVector& InLoc);
	UFUNCTION(BlueprintCallable, Category = "Label Base")
	void SetLabelVisiable(bool InShowUI);
public:
	//鼠标点击标签,触发聚焦事件
	UFUNCTION(BlueprintImplementableEvent)
	void FocusOnEvents(FVector InLoc);
	//鼠标点击标签,触发返回事件
	UFUNCTION(BlueprintImplementableEvent)
	void ReturnCamera();
	//创建详细面板事件
	UFUNCTION(BlueprintImplementableEvent)
	void CreateDetailPanel();
	//销毁详细面板事件
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveDetailPanel();

public:
	UFUNCTION(BlueprintCallable, Category = "Label Base")
	void OnClickEvent();
	UFUNCTION()
	void InitLabel(FVector InLoc, const FString& InType, bool bIsShow = true, bool InIsOpenDetail = false);
	UFUNCTION()
	void Remove();

	//virtual void AddComponentLayer(UUserWidget* InWidget, FName LayerName);
};

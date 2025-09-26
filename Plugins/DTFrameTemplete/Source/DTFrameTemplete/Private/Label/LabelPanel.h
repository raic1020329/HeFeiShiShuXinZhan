#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabelPanel.generated.h"
class UTextBlock;
class ULabelBase;

UCLASS()
class DTFRAMETEMPLETE_API ULabelPanel :public UUserWidget
{
	GENERATED_UCLASS_BODY()

public:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta = (BindWidget))
	class UCanvasPanel* MyCanvasPanel;
public:
	/**
	 * 创建标签
	 * @InLabelClass:标签的子类
	 * @InLocation:标签的位置
	 * @InType:标签的类型
	 * @bIsShow:创建出来的标签默认显示(bIsShow = true)
	 * @bIsOpenDetail:默认创建出来的标签不打开详细面板(bIsOpenDetail=false)
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	ULabelBase* CreateLabel(TSubclassOf<ULabelBase> InLabelClass, FVector InLocation, const FString& InType, bool bIsShow = true,bool bIsOpenDetail=false);

	/**
	 * 显示标签
	 * @InType：标签的ID编号
	 * @bOnlyShowThisLabel：如果为true表示只显示这类标签，反之不影响其他标签
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ShowLabelByType(const FString& InType,bool bOnlyShowThisLabel=false);
	/**
	 * 隐藏标签
	 * @InType：标签的ID编号
	 * @bOnlyShowThisLabel：如果为true表示只隐藏这类标签，反之不影响其他标签
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void HiddenLabelByType(const FString& InType, bool bOnlyHiddeThisLabel = false);

	/**
	 * 显示标签
	 * @InType：标签的ID编号
	* @bOnlyShowThisLabel：如果为true表示只显示这类标签，反之不影响其他标签
	*/
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ShowLabelByWidget(ULabelBase* Label,bool bOnlyShowThisLabel = false);
	/**
	 * 隐藏标签
	 * @InType：标签的ID编号
	 * @bOnlyShowThisLabel：如果为true表示只隐藏这类标签，反之不影响其他标签
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void HiddenLabelByWidget(ULabelBase* Label, bool bOnlyHiddeThisLabel = false);
	/**
	 * 显示全部标签
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ShowAllLabel(bool bIsShow);
	/**
	 * 删除全部标签
	 */
	UFUNCTION(BlueprintCallable,meta=(DisplayName="DeleteAllLabel"), Category = "Labe Panel")
	void ClearAll();
	/**
	 * 删除InType这一类的标签
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void DeleteLabelByType(const FString& InType);
	/**
	 * 返回相机的函数
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ReturnCamera();
	/*
	 *	获取全部标签的数量
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	int32 GetLabelNum() const;
	/**
	 * 判断是否有标签
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	bool HasLabel() const;
	/**
	 * 获取类型为InType的标签数量
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	int32 GetLabelNumByType(const FString& InType) const;
	/**
	 * 判断类型为InType的标签是否存在
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	bool HasLabelByType(const FString& InType) const;
	/**
	 * 获取类型为InType的标签对象，存储在OutLabelArray中
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	void GetLabelsByType(const FString& InType, TArray<ULabelBase*>& OutLabelArray)const;
	/**
	 * 获取全部标签对象
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	void GetLabels(TArray<ULabelBase*>& OutLabelArray)const;
public:
	UFUNCTION(BlueprintCallable)
	void LabelButtonCallBack(ULabelBase* InLabel);
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<ULabelBase*> LabelArray;
	UPROPERTY()
	ULabelBase* CurrentSelectLabel;
private:
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void SetZOrder(UWidget* Label, int32 InZOrder);

	TMap<FString, TSubclassOf<ULabelBase>> LabelClassMap;
};

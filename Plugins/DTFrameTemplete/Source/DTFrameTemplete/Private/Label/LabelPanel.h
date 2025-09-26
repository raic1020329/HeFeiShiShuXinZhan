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
	 * ������ǩ
	 * @InLabelClass:��ǩ������
	 * @InLocation:��ǩ��λ��
	 * @InType:��ǩ������
	 * @bIsShow:���������ı�ǩĬ����ʾ(bIsShow = true)
	 * @bIsOpenDetail:Ĭ�ϴ��������ı�ǩ������ϸ���(bIsOpenDetail=false)
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	ULabelBase* CreateLabel(TSubclassOf<ULabelBase> InLabelClass, FVector InLocation, const FString& InType, bool bIsShow = true,bool bIsOpenDetail=false);

	/**
	 * ��ʾ��ǩ
	 * @InType����ǩ��ID���
	 * @bOnlyShowThisLabel�����Ϊtrue��ʾֻ��ʾ�����ǩ����֮��Ӱ��������ǩ
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ShowLabelByType(const FString& InType,bool bOnlyShowThisLabel=false);
	/**
	 * ���ر�ǩ
	 * @InType����ǩ��ID���
	 * @bOnlyShowThisLabel�����Ϊtrue��ʾֻ���������ǩ����֮��Ӱ��������ǩ
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void HiddenLabelByType(const FString& InType, bool bOnlyHiddeThisLabel = false);

	/**
	 * ��ʾ��ǩ
	 * @InType����ǩ��ID���
	* @bOnlyShowThisLabel�����Ϊtrue��ʾֻ��ʾ�����ǩ����֮��Ӱ��������ǩ
	*/
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ShowLabelByWidget(ULabelBase* Label,bool bOnlyShowThisLabel = false);
	/**
	 * ���ر�ǩ
	 * @InType����ǩ��ID���
	 * @bOnlyShowThisLabel�����Ϊtrue��ʾֻ���������ǩ����֮��Ӱ��������ǩ
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void HiddenLabelByWidget(ULabelBase* Label, bool bOnlyHiddeThisLabel = false);
	/**
	 * ��ʾȫ����ǩ
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ShowAllLabel(bool bIsShow);
	/**
	 * ɾ��ȫ����ǩ
	 */
	UFUNCTION(BlueprintCallable,meta=(DisplayName="DeleteAllLabel"), Category = "Labe Panel")
	void ClearAll();
	/**
	 * ɾ��InType��һ��ı�ǩ
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void DeleteLabelByType(const FString& InType);
	/**
	 * ��������ĺ���
	 */
	UFUNCTION(BlueprintCallable, Category = "Labe Panel")
	void ReturnCamera();
	/*
	 *	��ȡȫ����ǩ������
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	int32 GetLabelNum() const;
	/**
	 * �ж��Ƿ��б�ǩ
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	bool HasLabel() const;
	/**
	 * ��ȡ����ΪInType�ı�ǩ����
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	int32 GetLabelNumByType(const FString& InType) const;
	/**
	 * �ж�����ΪInType�ı�ǩ�Ƿ����
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	bool HasLabelByType(const FString& InType) const;
	/**
	 * ��ȡ����ΪInType�ı�ǩ���󣬴洢��OutLabelArray��
	 */
	UFUNCTION(BlueprintPure, Category = "Labe Panel")
	void GetLabelsByType(const FString& InType, TArray<ULabelBase*>& OutLabelArray)const;
	/**
	 * ��ȡȫ����ǩ����
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

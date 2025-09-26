// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DTGameplay/DTControlInterface.h"
#include "UIBase.generated.h"

UENUM(BlueprintType)
namespace EUIAnimationMode
{
	enum Type
	{
		Opacity UMETA(DisplayName = "渐显渐隐"),
		SyncSlideIn UMETA(DisplayName = "同时滑入"),
		SyncSlideInWithOpacity UMETA(DisplayName = "同时滑入(带透明)"),		
		EnterInSequence UMETA(DisplayName = "先左后右由上而下"),
		EnterInSequenceWithOpacity UMETA(DisplayName = "先左后右由上而下(带透明)"),
		EnterInSync UMETA(DisplayName = "左右同时由上而下"),
		EnterInSyncWithOpacity UMETA(DisplayName = "左右同时由上而下(带透明)"),
		CustomAnim  UMETA(DisplayName = "自定义动画")
	};
}
class UCanvasPanel;
class UVerticalBox;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DTFRAMETEMPLETE_API UUIBase : public UUserWidget,public IDTControlInterface
{
	GENERATED_UCLASS_BODY()
public:
	virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	UFUNCTION(BlueprintCallable, Category = "UIBase")
	void PlayChangeUIAnim(EUMGSequencePlayMode::Type InPlayMode,float AnimLen,bool bRestart=false);
	UFUNCTION(BlueprintCallable, Category = "UIBase")
	void ChangeSubUIModule(FName InSubUIModuleTag);
	UFUNCTION(BlueprintCallable, Category = "UIBase")
	void ReturnParentUIModule(bool bIsExecParentAnim=true);
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UIBase")
	UCanvasPanel* RootCanvasPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UIBase")
	UCanvasPanel* SubUIContainerPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UIBase")
	UVerticalBox* LeftBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UIBase")
	UVerticalBox* RightBox;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "UIBase")
	UUIBase* CurrentSubUI;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBase")
	UUserWidget* ParentUI;
	virtual void DoCloseUI_Implementation() override;
	virtual void ReleaseOtherRes_Implementation() override;
	UFUNCTION()
	void ManualReleaseUI();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIBase")
	TMap<FName, TSubclassOf<UUIBase>> SubUIClassMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "动画模式"), Category = "UIBase")
	TEnumAsByte <EUIAnimationMode::Type> UIAnimationMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(Tooltip="动画时长"), Category = "UIBase")
	float AnimationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "左右面板宽度"), Category = "UIBase")
	float PanelWidth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBase")
	TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBase")
	FName ModuleTag;
protected:
	void ExectueAnimPlay(float InDeltaTime);
private:
	bool bAnimationStartPlay;
	float DeltaAnimTime;
	bool bIsDeleteUI;

	int32 CurrentAnimUIIndex;//当前动画的UI索引
	int32 ChildTotalCount;
private:
#if WITH_EDITOR
	FTimerHandle TimerHandle;
	UFUNCTION()
	void EditorAnimPreview();
#endif
};



// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIBase.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/World.h"
#include <DTGameplay/DTCommonFunLibrary.h>
#include "UI/MainUI.h"

#define PanelAnimLerp(Box,V,DeltaAnim) UCanvasPanelSlot* const PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Box);\
if (PanelSlot)\
{\
	float WithScaleWidth=PanelWidth;\
	float LerpValue = 0;\
	if (PlayMode == EUMGSequencePlayMode::Forward)\
	{\
		LerpValue = FMath::Lerp(V*WithScaleWidth-PanelSlot->GetOffsets().Left, 0.0f, DeltaAnim);\
	}\
	else\
	{\
		LerpValue = FMath::Lerp(0.f, V*WithScaleWidth-PanelSlot->GetOffsets().Left, DeltaAnim);\
	}\
	if(LerpValue!=0)\
	{\
	   Box->SetVisibility(ESlateVisibility::SelfHitTestInvisible); \
	 }\
	Box->SetRenderTranslation(FVector2D(LerpValue, 0));\
}

#define DTUIAnimationModeMacro(bIsOpacity,DeltaAnimTime) \
if (PlayMode == EUMGSequencePlayMode::Forward)\
{\
	if (CurrentAnimUIIndex < LeftBox->GetChildrenCount())\
	{\
		UWidget* Widget = LeftBox->GetChildAt(CurrentAnimUIIndex);\
		if (Widget)\
		{\
			Widget->SetRenderTranslation(FVector2D(-(1 - DeltaAnimTime) * PanelWidth, 0));\
			if(bIsOpacity)\
			{\
				Widget->SetRenderOpacity(DeltaAnimTime);\
			}\
		}\
	}\
	UWidget* Widget =nullptr;\
	if(UIAnimationMode==EUIAnimationMode::EnterInSync || UIAnimationMode==EUIAnimationMode::EnterInSyncWithOpacity)\
	{\
		Widget=RightBox->GetChildAt(CurrentAnimUIIndex);\
	}\
	else\
	{\
		if(CurrentAnimUIIndex >= LeftBox->GetChildrenCount())\
			Widget=RightBox->GetChildAt(CurrentAnimUIIndex - LeftBox->GetChildrenCount()); \
	}\
	if (Widget)\
	{\
		Widget->SetRenderTranslation(FVector2D((1 - DeltaAnimTime) * PanelWidth, 0));\
		if(bIsOpacity)\
		{\
			Widget->SetRenderOpacity(DeltaAnimTime);\
		}\
	}\
}\
else if (PlayMode == EUMGSequencePlayMode::Reverse)\
{\
	if (CurrentAnimUIIndex < LeftBox->GetChildrenCount())\
	{\
		UWidget* Widget = LeftBox->GetChildAt(CurrentAnimUIIndex);\
		if (Widget)\
		{\
			Widget->SetRenderTranslation(FVector2D(-DeltaAnimTime * PanelWidth, 0));\
			if(bIsOpacity)\
			{\
				Widget->SetRenderOpacity(1-DeltaAnimTime);\
			}\
		}\
	}\
	UWidget* Widget =nullptr;\
	if(UIAnimationMode==EUIAnimationMode::EnterInSync||UIAnimationMode==EUIAnimationMode::EnterInSyncWithOpacity)\
	{\
		Widget=RightBox->GetChildAt(CurrentAnimUIIndex);\
	}\
	else\
	{\
		if(CurrentAnimUIIndex >= LeftBox->GetChildrenCount())\
			Widget=RightBox->GetChildAt(CurrentAnimUIIndex - LeftBox->GetChildrenCount()); \
	}\
	if (Widget)\
	{\
		Widget->SetRenderTranslation(FVector2D(DeltaAnimTime * PanelWidth, 0));\
		if(bIsOpacity)\
		{\
			Widget->SetRenderOpacity(1-DeltaAnimTime);\
		}\
	}\
}

#define DTSyncSlideInMacro(DeltaAnimTime)\
if (LeftBox)\
{\
	PanelAnimLerp(LeftBox, -1, DeltaAnimTime)\
}\
if (RightBox)\
{\
	PanelAnimLerp(RightBox, 1, DeltaAnimTime)\
}\
if (UIAnimationMode == EUIAnimationMode::SyncSlideInWithOpacity)\
{\
	if (LeftBox && RightBox)\
	{\
		if (PlayMode == EUMGSequencePlayMode::Forward)\
		{\
			LeftBox->SetRenderOpacity(DeltaAnimTime);\
			RightBox->SetRenderOpacity(DeltaAnimTime);\
		}\
		else\
		{\
			LeftBox->SetRenderOpacity(1 - DeltaAnimTime);\
			RightBox->SetRenderOpacity(1 - DeltaAnimTime);\
		}\
	}\
}

UUIBase::UUIBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, UIAnimationMode(EUIAnimationMode::SyncSlideIn)
	, AnimationTime(0.5f)
	, PanelWidth(500)
{

}
void UUIBase::NativePreConstruct()
{
	bIsDeleteUI = false;
	bAnimationStartPlay = false;
	Super::NativePreConstruct();
	if (RootCanvasPanel)
	{
		if (LeftBox)
		{
			LeftBox->SetRenderTranslation(FVector2D(-1000, 0));
			UCanvasPanelSlot* const PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LeftBox);
			if (PanelSlot)
			{
				PanelSlot->SetAnchors(FAnchors(0, 0, 0, 1));
				PanelSlot->SetAutoSize(true);
				PanelSlot->SetAlignment(FVector2D(0, 0));
			}
		}
		if (RightBox)
		{
			RightBox->SetRenderTranslation(FVector2D(1000, 0));
			UCanvasPanelSlot* const PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RightBox);
			if (PanelSlot)
			{
				PanelSlot->SetAnchors(FAnchors(1, 0, 1, 1));
				PanelSlot->SetAutoSize(true);
				PanelSlot->SetAlignment(FVector2D(1, 0));
			}
		}
		if (SubUIContainerPanel)
		{
			UCanvasPanelSlot* const PanelSlot = RootCanvasPanel->AddChildToCanvas(SubUIContainerPanel);
			PanelSlot->SetAnchors(FAnchors(0, 0, 1, 1));
			PanelSlot->SetOffsets(0);
		}
	}
#if WITH_EDITOR
	if (!TimerHandle.IsValid())
	{
		UWorld* MyWorld = GetWorld();
		if (MyWorld && MyWorld->WorldType == EWorldType::EditorPreview)
		{
			PlayChangeUIAnim(PlayMode, AnimationTime, true);
			MyWorld->GetTimerManager().SetTimer(TimerHandle, this, &UUIBase::EditorAnimPreview, 0.002f, true);
		}
	}
#endif
}
void UUIBase::NativeConstruct()
{
	PlayChangeUIAnim(EUMGSequencePlayMode::Forward, AnimationTime, true);
	Super::NativeConstruct();

}

void UUIBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUIBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ExectueAnimPlay(InDeltaTime);
}

void UUIBase::PlayChangeUIAnim(EUMGSequencePlayMode::Type InPlayMode, float AnimLen, bool bRestart/* = false*/)
{
	if (!bAnimationStartPlay || bRestart)
	{
		if (UIAnimationMode != EUIAnimationMode::SyncSlideIn && UIAnimationMode != EUIAnimationMode::CustomAnim)
		{
			LeftBox->SetRenderTranslation(FVector2D(0, 0));
			RightBox->SetRenderTranslation(FVector2D(0, 0));
		}
		if (UIAnimationMode == EUIAnimationMode::EnterInSequence || UIAnimationMode == EUIAnimationMode::EnterInSync ||
			UIAnimationMode == EUIAnimationMode::EnterInSequenceWithOpacity || UIAnimationMode == EUIAnimationMode::EnterInSyncWithOpacity)
		{
			CurrentAnimUIIndex = 0;
			if (UIAnimationMode == EUIAnimationMode::EnterInSync || UIAnimationMode == EUIAnimationMode::EnterInSyncWithOpacity)
			{
				ChildTotalCount = LeftBox->GetChildrenCount() > RightBox->GetChildrenCount() ? LeftBox->GetChildrenCount() : RightBox->GetChildrenCount();
			}
			else
			{
				ChildTotalCount = LeftBox->GetChildrenCount() + RightBox->GetChildrenCount();
			}
			for (UWidget* Widget : LeftBox->GetAllChildren())
			{
				if (Widget)
				{
					if (InPlayMode == EUMGSequencePlayMode::Forward)
					{
						Widget->SetRenderTranslation(FVector2D(-PanelWidth, 0));
					}
					else
					{
						Widget->SetRenderTranslation(FVector2D(0, 0));
					}

				}
			}
			for (UWidget* Widget : RightBox->GetAllChildren())
			{
				if (Widget)
				{
					if (InPlayMode == EUMGSequencePlayMode::Forward)
					{
						Widget->SetRenderTranslation(FVector2D(PanelWidth, 0));
					}
					else
					{
						Widget->SetRenderTranslation(FVector2D(0, 0));
					}
				}
			}
		}
		if (PlayMode != InPlayMode || bRestart)
		{
			PlayMode = InPlayMode;
			AnimationTime = AnimLen;
			AnimationTime = FMath::Clamp(AnimationTime, 0.f, AnimationTime);
			DeltaAnimTime = 0;
			bAnimationStartPlay = true;
		}
	}
	else
	{
		if (PlayMode != InPlayMode)
		{
			ChildTotalCount = CurrentAnimUIIndex + 1;
			PlayMode = InPlayMode;
			AnimationTime = AnimLen;
			AnimationTime = FMath::Clamp(AnimationTime, 0.f, AnimationTime);
			if (UIAnimationMode == EUIAnimationMode::EnterInSequence || UIAnimationMode == EUIAnimationMode::EnterInSync)
			{
				DTUIAnimationModeMacro(false, 0)
			}
			else if (UIAnimationMode == EUIAnimationMode::EnterInSequenceWithOpacity || UIAnimationMode == EUIAnimationMode::EnterInSyncWithOpacity)
			{
				DTUIAnimationModeMacro(true, 0)
			}
			CurrentAnimUIIndex = 0;
			bAnimationStartPlay = true;
		}
	}
}

void UUIBase::ChangeSubUIModule(FName InSubUIModuleTag)
{
	if (ModuleTag != InSubUIModuleTag)
	{
		ModuleTag = InSubUIModuleTag;
		//隐藏自己左右两边的面板
		PlayChangeUIAnim(EUMGSequencePlayMode::Reverse, AnimationTime);
		if (SubUIContainerPanel)
		{
			for (UWidget* UIItem : SubUIContainerPanel->GetAllChildren())
			{
				if (UIItem && UIItem->IsValidLowLevel())
				{
					if (UIItem->GetVisibility() == ESlateVisibility::Collapsed || UIItem->GetVisibility() == ESlateVisibility::Hidden)
					{
						UIItem->RemoveFromParent();
						SubUIContainerPanel->RemoveChild(UIItem);
					}
					else
					{
						IDTControlInterface::Execute_DoCloseUI(UIItem);
					}
				}
				else
				{
					SubUIContainerPanel->RemoveChild(UIItem);
				}
			}
		}
		if (SubUIClassMap.Contains(InSubUIModuleTag))
		{
			//SubUIClassMap[InSubUIModuleIndex]有意义且这个类不能等于它自己（防止套娃）
			if (SubUIClassMap[InSubUIModuleTag] && !this->IsA(SubUIClassMap[InSubUIModuleTag]))
			{
				CurrentSubUI = CreateWidget<UUIBase>(GetWorld(), SubUIClassMap[InSubUIModuleTag]);
				if (SubUIContainerPanel)
				{
					CurrentSubUI->ParentUI = this;
					UCanvasPanelSlot* PanelSlot = SubUIContainerPanel->AddChildToCanvas(CurrentSubUI);
					if (PanelSlot)
					{
						PanelSlot->SetAnchors(FAnchors(0, 0, 1, 1));
						PanelSlot->SetOffsets(0);
					}
				}
			}
			else
			{
				CurrentSubUI = nullptr;
			}
		}
	}

	//切换相机
	UDTCommonFunLibrary::ChangeCameraByTag(GetWorld(), InSubUIModuleTag);
}

void UUIBase::ReturnParentUIModule(bool bIsExecParentAnim)
{
	if (ParentUI)
	{
		Execute_DoCloseUI(this);
		if (UUIBase* UIItem = Cast<UUIBase>(ParentUI))
		{
			Execute_ReturnParentUIEvent(UIItem);
			UIItem->ModuleTag = FName(TEXT("NONE-1"));
			if (bIsExecParentAnim)
			{
				UIItem->PlayChangeUIAnim(EUMGSequencePlayMode::Forward, AnimationTime);
			}
		}

	}
}

void UUIBase::DoCloseUI_Implementation()
{
	if (!bIsDeleteUI)
	{
		bIsDeleteUI = true;
		ManualReleaseUI();
		if (GetVisibility() == ESlateVisibility::Collapsed || GetVisibility() == ESlateVisibility::Hidden)
		{
			RemoveFromParent();
		}
	}
}

void UUIBase::ReleaseOtherRes_Implementation()
{
}

void UUIBase::ManualReleaseUI()
{
	Execute_ReleaseOtherRes(this);
	for (UWidget* UIItem : SubUIContainerPanel->GetAllChildren())
	{
		UUIBase* UIBase = Cast<UUIBase>(UIItem);
		if (UIBase && UIBase->IsValidLowLevel())
		{
			Execute_DoCloseUI(UIBase);
		}
	}
	if (PlayMode == EUMGSequencePlayMode::Reverse)
	{
		DeltaAnimTime = 0.f;
		bAnimationStartPlay = true;
		/*if (bIsDeleteUI)
		{
			RemoveFromParent();
			bIsDeleteUI = false;
		}*/
	}
	else
	{
		PlayChangeUIAnim(EUMGSequencePlayMode::Reverse, AnimationTime);
	}


}

void UUIBase::ExectueAnimPlay(float InDeltaTime)
{
	if (bAnimationStartPlay)
	{
		DeltaAnimTime += InDeltaTime / AnimationTime;
		if (DeltaAnimTime <= 1.f)
		{
			if (UIAnimationMode == EUIAnimationMode::SyncSlideIn || UIAnimationMode == EUIAnimationMode::SyncSlideInWithOpacity)
			{
				DTSyncSlideInMacro(DeltaAnimTime)
			}
			else if (UIAnimationMode == EUIAnimationMode::Opacity)
			{
				if (LeftBox && RightBox)
				{
					if (PlayMode == EUMGSequencePlayMode::Forward)
					{
						LeftBox->SetRenderOpacity(DeltaAnimTime);
						RightBox->SetRenderOpacity(DeltaAnimTime);
					}
					else
					{
						LeftBox->SetRenderOpacity(1 - DeltaAnimTime);
						RightBox->SetRenderOpacity(1 - DeltaAnimTime);
					}

				}
			}
			else if (UIAnimationMode == EUIAnimationMode::EnterInSequence || UIAnimationMode == EUIAnimationMode::EnterInSync)
			{
				DTUIAnimationModeMacro(false, DeltaAnimTime)
			}
			else if (UIAnimationMode == EUIAnimationMode::EnterInSequenceWithOpacity || UIAnimationMode == EUIAnimationMode::EnterInSyncWithOpacity)
			{
				DTUIAnimationModeMacro(true, DeltaAnimTime)
			}
		}
		else
		{
			if (UIAnimationMode == EUIAnimationMode::SyncSlideIn || UIAnimationMode == EUIAnimationMode::SyncSlideInWithOpacity)
			{
				DTSyncSlideInMacro(1.0f)
			}
			else if (UIAnimationMode == EUIAnimationMode::Opacity)
			{
				if (LeftBox && RightBox)
				{
					if (PlayMode == EUMGSequencePlayMode::Forward)
					{
						LeftBox->SetRenderOpacity(1);
						RightBox->SetRenderOpacity(1);
					}
					else
					{
						LeftBox->SetRenderOpacity(0);
						RightBox->SetRenderOpacity(0);
					}
				}
			}
			else if (UIAnimationMode == EUIAnimationMode::EnterInSequence || UIAnimationMode == EUIAnimationMode::EnterInSync)
			{
				DTUIAnimationModeMacro(false, 1)
			}
			else if (UIAnimationMode == EUIAnimationMode::EnterInSequenceWithOpacity || UIAnimationMode == EUIAnimationMode::EnterInSyncWithOpacity)
			{
				DTUIAnimationModeMacro(true, 1)
			}
			if (UIAnimationMode == EUIAnimationMode::EnterInSequence || UIAnimationMode == EUIAnimationMode::EnterInSync ||
				UIAnimationMode == EUIAnimationMode::EnterInSyncWithOpacity || UIAnimationMode == EUIAnimationMode::EnterInSequenceWithOpacity)
			{
				DeltaAnimTime = 0.f;
				bool bSuccess = false;
				CurrentAnimUIIndex++;
				if (CurrentAnimUIIndex == ChildTotalCount)
				{
					bSuccess = true;
				}
				if (bSuccess)
				{
					bAnimationStartPlay = false;
					if (bIsDeleteUI)
					{
						RemoveFromParent();
						bIsDeleteUI = false;
					}
				}
			}
			else
			{
				DeltaAnimTime = 0.f;
				bAnimationStartPlay = false;
				if (bIsDeleteUI)
				{
					RemoveFromParent();
					bIsDeleteUI = false;
				}
			}
		}

	}
}

#if WITH_EDITOR
void UUIBase::EditorAnimPreview()
{
	ExectueAnimPlay(0.002);
	if (!bAnimationStartPlay)
	{
		UWorld* MyWorld = GetWorld();
		if (MyWorld && MyWorld->WorldType == EWorldType::EditorPreview)
		{
			MyWorld->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}
#endif

#undef PanelAnimLerp
#undef DTUIAnimationModeMacro
#undef DTSyncSlideInMacro
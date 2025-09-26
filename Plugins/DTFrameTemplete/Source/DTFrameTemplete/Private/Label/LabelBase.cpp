#include "Label/LabelBase.h"
#include "Components/Button.h"
//#include <Slate/SGameLayerManager.h>
//#include <Slate/SWorldWidgetScreenLayer.h>

ULabelBase::ULabelBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsOpenDetailPanel = false;
	LabelType = 0;
	bIsShowUI = true;
}

void ULabelBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (MyButton)
	{
		MyButton->OnClicked.AddDynamic(this, &ULabelBase::OnClickEvent);
	}
}

void ULabelBase::NativeDestruct()
{
	Super::NativeDestruct();
	if (MyButton)
	{
		MyButton->OnClicked.RemoveAll(this);
	}
}

void ULabelBase::UpdateLocation(const FVector& InLoc)
{
	Location = InLoc;
}


void ULabelBase::SetLabelVisiable(bool InShowUI)
{
	if (bIsShowUI == InShowUI)
		return;
	bIsShowUI = InShowUI;
	if (bIsShowUI)
	{
		SetRenderOpacity(1.f);
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetRenderOpacity(0.f);
		SetVisibility(ESlateVisibility::Collapsed);
	}

}

void ULabelBase::OnClickEvent()
{
	//FocusOnEvents();
	LabelBaseDelegate.ExecuteIfBound(this);
}

void ULabelBase::InitLabel(FVector InLoc, const FString& InType/* = 0*/, bool bIsShow/* = true*/, bool InIsOpenDetail/* = false*/)
{
	Location = InLoc;
	LabelType = InType;
	bIsOpenDetailPanel = InIsOpenDetail;
	SetLabelVisiable(bIsShow);
	if (bIsOpenDetailPanel)
	{
		LabelBaseDelegate.ExecuteIfBound(this);
	}
}

void ULabelBase::Remove()
{
	RemoveDetailPanel();
	RemoveFromParent();
}

//void ULabelBase::AddComponentLayer(UUserWidget* InWidget, FName LayerName)
//{
//	if (UGameViewportClient* ViewportClient = GWorld->GetGameViewport())
//	{
//		TSharedPtr<IGameLayerManager> LayerManager = ViewportClient->GetGameLayerManager();
//
//		if (LayerManager.IsValid())
//		{
//			UGameInstance* GameInstance = GWorld->GetGameInstance();
//			check(GameInstance);
//			if (ULocalPlayer* TargetPlayer = GameInstance->GetFirstGamePlayer())
//			{
//				TSharedPtr<IGameLayer> Layer = LayerManager->FindLayerForPlayer(TargetPlayer, LayerName);
//				if (Layer.IsValid())
//				{
//					TSharedRef<SWorldWidgetScreenLayer> NewScreenLayer = StaticCastSharedRef<SWorldWidgetScreenLayer>(Layer->AsWidget());
//					if (NewScreenLayer->DoesSharedInstanceExist())
//					{
//						SConstraintCanvas::FSlot* Slot = nullptr;
//						NewScreenLayer->GetCanvas()->AddSlot()
//							.Expose(Slot)
//							[
//								InWidget->TakeWidget()
//							];
//					}
//				}
//			}
//		}
//	}
//}


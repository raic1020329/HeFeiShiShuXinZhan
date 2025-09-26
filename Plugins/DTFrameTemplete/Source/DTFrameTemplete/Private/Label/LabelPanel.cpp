#include "Label/LabelPanel.h"
#include "Label/LabelBase.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"


ULabelPanel::ULabelPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULabelPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULabelPanel::NativeDestruct()
{
	Super::NativeDestruct();
	ClearAll();
}

void ULabelPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
	for (ULabelBase* Label : LabelArray)
	{
		UWorld* const MyWorld = GetWorld();
		APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(MyWorld, 0);
		if (PlayerController && Label->IsValidLowLevel())
		{
			FVector2D ScreenPosition;
			bool success = PlayerController->ProjectWorldLocationToScreen(Label->Location, ScreenPosition);
			if (success)
			{
				const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(MyWorld);
				const float RelativeScale = FMath::Pow(ViewportScale, -1);
				ScreenPosition = ScreenPosition * RelativeScale;
				UCanvasPanelSlot* const PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Label);
				if (PanelSlot)
				{
					PanelSlot->SetPosition(ScreenPosition);
					if (Label->bIsShowUI)//如果UI是显示状态才会触发
					{
						Label->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						Label->SetRenderOpacity(1.f);
					}

				}
			}
			else
			{
				Label->SetRenderOpacity(0.f);
				Label->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
}

ULabelBase* ULabelPanel::CreateLabel(TSubclassOf<ULabelBase> InLabelClass, FVector InLocation, const FString& InType/* = 0*/, bool bIsShow/* = true*/, bool bIsOpenDetail/* = false*/)
{

	ULabelBase* UI_Label = CreateWidget<ULabelBase>(this, InLabelClass);
	if (UI_Label)
	{
		UCanvasPanelSlot* Solt = MyCanvasPanel->AddChildToCanvas(UI_Label);
		if (Solt)
		{
			Solt->SetAutoSize(true);
		}
		UI_Label->InitLabel(InLocation, InType, bIsShow, bIsOpenDetail);
		UI_Label->LabelBaseDelegate.BindUObject(this, &ULabelPanel::LabelButtonCallBack);
		LabelArray.Add(UI_Label);
		return UI_Label;
	}
	return nullptr;
}

void ULabelPanel::ShowLabelByType(const FString& InType, bool bOnlyShowThisLabel/* = false*/)
{
	for (ULabelBase* Label : LabelArray)
	{
		if (Label->LabelType == InType)
		{
			Label->SetLabelVisiable(1);
		}
		else
		{
			if (bOnlyShowThisLabel)
			{
				Label->SetLabelVisiable(0);
			}
		}
	}
}

void ULabelPanel::HiddenLabelByType(const FString& InType, bool bOnlyHiddeThisLabel/* = false*/)
{
	for (ULabelBase* Label : LabelArray)
	{
		if (Label->LabelType == InType)
		{
			Label->SetLabelVisiable(0);
		}
		else
		{
			if (bOnlyHiddeThisLabel)
			{
				Label->SetLabelVisiable(1);
			}
		}
	}
}

void ULabelPanel::ShowLabelByWidget(ULabelBase* Label, bool bOnlyShowThisLabel /*= false*/)
{
	if (Label)
	{
		Label->SetLabelVisiable(1);
	}
}

void ULabelPanel::HiddenLabelByWidget(ULabelBase* Label, bool bOnlyHiddeThisLabel /*= false*/)
{
	if (Label)
	{
		Label->SetLabelVisiable(0);
	}
}

void ULabelPanel::ShowAllLabel(bool bIsShow)
{
	for (ULabelBase* Label : LabelArray)
	{
		Label->SetLabelVisiable(bIsShow);
	}
}

void ULabelPanel::ClearAll()
{
	for (ULabelBase* Label : LabelArray)
	{
		Label->Remove();
	}
}

void ULabelPanel::DeleteLabelByType(const FString& InType)
{
	for (ULabelBase* Label : LabelArray)
	{
		if (Label->LabelType == InType)
			Label->Remove();
	}

}

void ULabelPanel::ReturnCamera()
{
	LabelButtonCallBack(CurrentSelectLabel);
}

int32 ULabelPanel::GetLabelNum() const
{
	return LabelArray.Num();
}

bool ULabelPanel::HasLabel() const
{
	return GetLabelNum() > 0 ? 1 : 0;
}

int32 ULabelPanel::GetLabelNumByType(const FString& InType) const
{
	int32 num = 0;
	for (ULabelBase const* const Label : LabelArray)
	{
		if (Label->LabelType == InType)
			num++;
	}
	return num;
}

bool ULabelPanel::HasLabelByType(const FString& InType) const
{
	return GetLabelNumByType(InType) > 0 ? 1 : 0;
}

void ULabelPanel::GetLabelsByType(const FString& InType, TArray<ULabelBase*>& OutLabelArray) const
{
	for (ULabelBase* Label : LabelArray)
	{
		if (Label && Label->LabelType == InType)
		{
			OutLabelArray.Add(Label);
		}
	}
}

void ULabelPanel::GetLabels(TArray<ULabelBase*>& OutLabelArray) const
{
	OutLabelArray = LabelArray;
}

void ULabelPanel::LabelButtonCallBack(ULabelBase* InLabel)
{
	for (ULabelBase* Label : LabelArray)
	{
		if (Label->IsValidLowLevel())
		{
			if (Label == InLabel)
			{
				if (Label->bIsOpenDetailPanel)
				{
					//触发返回事件
					Label->bIsOpenDetailPanel = false;
					Label->ReturnCamera();
					//移除详细面板
					Label->RemoveDetailPanel();
					SetZOrder(Label, 0);
					CurrentSelectLabel = nullptr;
				}
				else
				{
					//触发聚焦事件
					Label->bIsOpenDetailPanel = true;
					Label->FocusOnEvents(Label->Location);
					CurrentSelectLabel = InLabel;
					//创建详细面板
					Label->CreateDetailPanel();
					SetZOrder(Label, 1);
				}
			}
			else
			{
				Label->bIsOpenDetailPanel = false;
				//如果不是当前点击的Label, 移除它的详细面板
				Label->RemoveDetailPanel();
				SetZOrder(Label, 0);
			}
		}

	}
}

void ULabelPanel::SetZOrder(UWidget* Label, int32 InZOrder)
{
	UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Label);
	if (PanelSlot)
	{
		PanelSlot->SetZOrder(InZOrder);
	}
}

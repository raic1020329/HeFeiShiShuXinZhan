// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"
#include "UI/UIBase.h"
#include "DTGameplay/DTCommonFunLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UMainUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (RootCanvasPanel && UIContainerPanel)
	{
		RootCanvasPanel->AddChildToCanvas(UIContainerPanel);

		UCanvasPanelSlot* const PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(UIContainerPanel);
		if (PanelSlot)
		{
			PanelSlot->SetAnchors(FAnchors(0, 0, 1, 1));
			PanelSlot->SetOffsets(FMargin(0));
		}
	}
}

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUI::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

}

void UMainUI::ChangeModule(FName InModuleTag)
{
	ModuleTag = InModuleTag;
	if (UIContainerPanel)
	{
		for (UWidget* UIItem : UIContainerPanel->GetAllChildren())
		{
			if (UIItem && UIItem->IsValidLowLevel())
			{
				if (UIItem->GetVisibility() == ESlateVisibility::Collapsed || UIItem->GetVisibility() == ESlateVisibility::Hidden)
				{
					UIItem->RemoveFromParent();
					UIContainerPanel->RemoveChild(UIItem);
				}
				else
				{
					IDTControlInterface::Execute_DoCloseUI(UIItem);
				}
			}
			else
			{
				UIContainerPanel->RemoveChild(UIItem);
			}
		}
	}
	if (UIBaseClassMap.Contains(InModuleTag))
	{
		if (UIBaseClassMap[InModuleTag])
		{
			CurrentUIModule = CreateWidget<UUIBase>(GetWorld(), UIBaseClassMap[InModuleTag]);
			if (UIContainerPanel)
			{
				CurrentUIModule->ParentUI = this;
				UCanvasPanelSlot* PanelSlot = UIContainerPanel->AddChildToCanvas(CurrentUIModule);
				if (PanelSlot)
				{
					PanelSlot->SetAnchors(FAnchors(0, 0, 1, 1));
					PanelSlot->SetOffsets(0);
				}
			}
		}
		else
		{
			CurrentUIModule = nullptr;
		}
	}

	UDTCommonFunLibrary::ChangeCameraByTag(GetWorld(), InModuleTag);
}


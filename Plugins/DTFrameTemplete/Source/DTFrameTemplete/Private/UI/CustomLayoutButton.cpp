#include "UI/CustomLayoutButton.h"
#include "Components/CanvasPanelSlot.h"

#define LOCTEXT_NAMESPACE "DTUI"

UCustomLayoutButton::UCustomLayoutButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UCustomLayoutButton::OnButtonClick(int32 Index)
{
	ButtonLeafCallBack(Index);
}

#if WITH_EDITOR
const FText UCustomLayoutButton::GetPaletteCategory()
{
	return LOCTEXT("DTUI", "DTUI");
}
#endif

UClass* UCustomLayoutButton::GetSlotClass() const
{
	return UCanvasPanelSlot::StaticClass();
}

void UCustomLayoutButton::OnSlotAdded(UPanelSlot* InSlot)
{
	if (MyCanvas.IsValid())
	{
		CastChecked<UCanvasPanelSlot>(InSlot)->BuildSlot(MyCanvas.ToSharedRef());
	}
	if (InSlot->Content)
	{
		if (!InSlot->Content->IsA(UDTButtonBase::StaticClass()))
		{
			OnSlotRemoved(InSlot);
			Slots.Remove(InSlot);
		}
		else
		{
			if (UDTButtonBase* BtnLeaf = Cast<UDTButtonBase>(InSlot->Content))
			{
				if (!BtnLeaf->ButtonOnClikDelegate.IsBound())
				{
					BtnLeaf->ButtonOnClikDelegate.BindUObject(this, &UCustomLayoutButton::ButtonLeafCallBack);
				}
			}
		}
	}
}

void UCustomLayoutButton::OnSlotRemoved(UPanelSlot* InSlot)
{
	if (InSlot->Content)
	{	
		// Remove the widget from the live slot if it exists.
		if (MyCanvas.IsValid())
		{
			TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
			if (Widget.IsValid())
			{
				if (InSlot->Content->IsA(UDTButtonBase::StaticClass()))
				{
					if (UDTButtonBase* BtnLeaf = Cast<UDTButtonBase>(InSlot->Content))
					{
						BtnLeaf->ButtonOnClikDelegate.Unbind();
					}
				}

				MyCanvas->RemoveSlot(Widget.ToSharedRef());
			}
		}
	}
}

void UCustomLayoutButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyCanvas.Reset();
}

void UCustomLayoutButton::ButtonLeafCallBack(int32 Index)
{
	bool bHasButton = false;
	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UDTButtonBase* BtnLeaf = Cast<UDTButtonBase>(PanelSlot->Content))
		{
			if (ButtonOnClickType == EDTButtonOnClickType::FreeButton)
			{
				if (BtnLeaf->ButtonIndex != Index)
				{
					continue;
				}
			}
			if (BtnLeaf->ButtonIndex == Index)
			{
				if (ButtonOnClickType == EDTButtonOnClickType::CheckButton)
				{
					BtnLeaf->ResertButton(ButtonOnClickType, !BtnLeaf->IsSelect);
					bHasButton = true;
					LayoutButtonDelegate.Broadcast(BtnLeaf->ButtonIndex, BtnLeaf->IsSelect);
				}
				else if (ButtonOnClickType == EDTButtonOnClickType::MutexButton)
				{
					BtnLeaf->ResertButton(ButtonOnClickType, true);
					bHasButton = true;
					LayoutButtonDelegate.Broadcast(BtnLeaf->ButtonIndex, BtnLeaf->IsSelect);
				}
				else
				{
					BtnLeaf->ResertButton(ButtonOnClickType, false);
					bHasButton = true;
					LayoutButtonDelegate.Broadcast(BtnLeaf->ButtonIndex, BtnLeaf->IsSelect);
					break;
				}

			}
			else
			{
				if (ButtonOnClickType == EDTButtonOnClickType::MutexButton)
				{
					BtnLeaf->ResertButton(ButtonOnClickType, false);
				}
			}

		}
	}
	if (!bHasButton)
	{
		LayoutButtonDelegate.Broadcast(Index, false);
	}
}

TSharedRef<SWidget> UCustomLayoutButton::RebuildWidget()
{
	MyCanvas = SNew(SConstraintCanvas);
	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UCanvasPanelSlot* TypedSlot = Cast<UCanvasPanelSlot>(PanelSlot))
		{
			if (UDTButtonBase* BtnLeaf = Cast<UDTButtonBase>(TypedSlot->Content))
			{
				if (!BtnLeaf->ButtonOnClikDelegate.IsBound())
				{
					BtnLeaf->ButtonOnClikDelegate.BindUObject(this, &UCustomLayoutButton::ButtonLeafCallBack);
				}
				TypedSlot->Parent = this;
				TypedSlot->BuildSlot(MyCanvas.ToSharedRef());
			}
		}
	}
	return MyCanvas.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE

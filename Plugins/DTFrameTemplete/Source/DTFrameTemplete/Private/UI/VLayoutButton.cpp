#include "UI/VLayoutButton.h"
#include "Components/VerticalBoxSlot.h"

#define LOCTEXT_NAMESPACE "DTUI"

UVLayoutButton::UVLayoutButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible) ;
}

void UVLayoutButton::OnButtonClick(int32 Index)
{
	ButtonLeafCallBack(Index);
}

#if WITH_EDITOR
const FText UVLayoutButton::GetPaletteCategory()
{
	return LOCTEXT("DTUI", "DTUI");
}
#endif

UClass* UVLayoutButton::GetSlotClass() const
{
	return UVerticalBoxSlot::StaticClass();
}

void UVLayoutButton::OnSlotAdded(UPanelSlot* InSlot)
{
	if (MyVerticalBox.IsValid())
	{
		CastChecked<UVerticalBoxSlot>(InSlot)->BuildSlot(MyVerticalBox.ToSharedRef());
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
					BtnLeaf->ButtonOnClikDelegate.BindUObject(this, &UVLayoutButton::ButtonLeafCallBack);
				}
			}
		}
	}
}

void UVLayoutButton::OnSlotRemoved(UPanelSlot* InSlot)
{
	if (InSlot->Content)
	{
		// Remove the widget from the live slot if it exists.
		if (MyVerticalBox.IsValid())
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
				MyVerticalBox->RemoveSlot(Widget.ToSharedRef());
			}
		}
	}
}

void UVLayoutButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyVerticalBox.Reset();
}

void UVLayoutButton::ButtonLeafCallBack(int32 Index)
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

TSharedRef<SWidget> UVLayoutButton::RebuildWidget()
{
	MyVerticalBox = SNew(SVerticalBox);
	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UVerticalBoxSlot* TypedSlot = Cast<UVerticalBoxSlot>(PanelSlot))
		{
			if (UDTButtonBase* BtnLeaf = Cast<UDTButtonBase>(TypedSlot->Content))
			{
				if (!BtnLeaf->ButtonOnClikDelegate.IsBound())
				{
					BtnLeaf->ButtonOnClikDelegate.BindUObject(this, &UVLayoutButton::ButtonLeafCallBack);
				}

				TypedSlot->Parent = this;
				TypedSlot->BuildSlot(MyVerticalBox.ToSharedRef());
			}
		}
	}
	return MyVerticalBox.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE

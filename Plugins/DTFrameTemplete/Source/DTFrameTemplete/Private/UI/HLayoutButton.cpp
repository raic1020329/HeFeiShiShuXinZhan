#include "UI/HLayoutButton.h"
#include "Components/HorizontalBoxSlot.h"

#define LOCTEXT_NAMESPACE "DTUI"

UHLayoutButton::UHLayoutButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
}
void UHLayoutButton::OnButtonClick(int32 Index)
{
	ButtonLeafCallBack(Index);
}

#if WITH_EDITOR
const FText UHLayoutButton::GetPaletteCategory()
{
	return LOCTEXT("DTUI", "DTUI");
}
#endif

UClass* UHLayoutButton::GetSlotClass() const
{
	return UHorizontalBoxSlot::StaticClass();
}

void UHLayoutButton::OnSlotAdded(UPanelSlot* InSlot)
{
	if (MyHorizontalBox.IsValid() && InSlot->Content)
	{
		CastChecked<UHorizontalBoxSlot>(InSlot)->BuildSlot(MyHorizontalBox.ToSharedRef());
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
					BtnLeaf->ButtonOnClikDelegate.BindUObject(this, &UHLayoutButton::ButtonLeafCallBack);
				}
			}
		}
	}
}

void UHLayoutButton::OnSlotRemoved(UPanelSlot* InSlot)
{
	if (InSlot->Content)
	{
		// Remove the widget from the live slot if it exists.
		if (MyHorizontalBox.IsValid())
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
				

				MyHorizontalBox->RemoveSlot(Widget.ToSharedRef());
			}
		}
	}
}

void UHLayoutButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyHorizontalBox.Reset();
}

void UHLayoutButton::ButtonLeafCallBack(int32 Index)
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
					BtnLeaf->ResertButton(ButtonOnClickType,!BtnLeaf->IsSelect);
					bHasButton = true;
					LayoutButtonDelegate.Broadcast(BtnLeaf->ButtonIndex, BtnLeaf->IsSelect);
				}
				else if(ButtonOnClickType == EDTButtonOnClickType::MutexButton)
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
					BtnLeaf->ResertButton(ButtonOnClickType,false);
				}
			}	
		
		}
	}
	if (!bHasButton)
	{
		LayoutButtonDelegate.Broadcast(Index, false);
	}
}

TSharedRef<SWidget> UHLayoutButton::RebuildWidget()
{
	MyHorizontalBox = SNew(SHorizontalBox);
	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UHorizontalBoxSlot* TypedSlot = Cast<UHorizontalBoxSlot>(PanelSlot))
		{
			if (UDTButtonBase* BtnLeaf = Cast<UDTButtonBase>(TypedSlot->Content))
			{
				if (!BtnLeaf->ButtonOnClikDelegate.IsBound())
				{
					BtnLeaf->ButtonOnClikDelegate.BindUObject(this, &UHLayoutButton::ButtonLeafCallBack);
				}			
				TypedSlot->Parent = this;
				TypedSlot->BuildSlot(MyHorizontalBox.ToSharedRef());
			}
		}
	}
	return MyHorizontalBox.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE

#include "UI/ButtonBase.h"

static FButtonStyle* DefaultButtonStyle = nullptr;

UDTButtonBase::UDTButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultButtonStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
#if WITH_EDITOR
		DefaultButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));
		// Unlink UMG default colors from the editor settings colors.
		DefaultButtonStyle->UnlinkColors();
#endif
	}
	if (DefaultButtonStyle)
	{
		ButtonStyle.Normal = DefaultButtonStyle->Normal;
		ButtonStyle.Hovered = DefaultButtonStyle->Hovered;
		ButtonStyle.Select = DefaultButtonStyle->Hovered;
		ButtonStyle.Pressed = DefaultButtonStyle->Pressed;
	}

	IsSelect = false;
}

void UDTButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (MyButton)
	{
		MyButton->bOverride_Cursor = true;
		MyButton->SetCursor(EMouseCursor::Hand);
		FButtonStyle Style = MyButton->GetStyle();
		Style.SetNormal(ButtonStyle.Normal);
		Style.SetHovered(ButtonStyle.Hovered);
		Style.SetPressed(ButtonStyle.Pressed);
		MyButton->SetStyle(Style);
		if (MyText)
		{
			MyText->SetText(ButtonName);
			MyText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

}

void UDTButtonBase::NativeConstruct()
{
	if (MyButton)
	{
		MyButton->OnClicked.AddDynamic(this, &UDTButtonBase::OnClickedCallBack);
		MyButton->OnHovered.AddDynamic(this, &UDTButtonBase::OnHoveredCallBack);
		MyButton->OnUnhovered.AddDynamic(this, &UDTButtonBase::OnUnHoveredCallBack);
		MyButton->OnPressed.AddDynamic(this, &UDTButtonBase::OnPressCallBack);
	}
	Super::NativeConstruct();
}

void UDTButtonBase::NativeDestruct()
{
	Super::NativeDestruct();

	MyButton->OnClicked.RemoveAll(this);
	MyButton->OnHovered.RemoveAll(this);
	MyButton->OnUnhovered.RemoveAll(this);
	MyButton->OnPressed.RemoveAll(this);
}

void UDTButtonBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDTButtonBase::ResertButton(EDTButtonOnClickType InType, bool bIsSelect)
{
	OnClickType = InType;
	IsSelect = bIsSelect;
	if (bIsSelect)
	{
		if (OnClickType != EDTButtonOnClickType::FreeButton)
		{
			FButtonStyle Style = MyButton->GetStyle();
			Style.SetNormal(ButtonStyle.Select);
			MyButton->SetStyle(Style);
		}
		if (OnClickType == EDTButtonOnClickType::MutexButton)
		{
			MyButton->SetVisibility(ESlateVisibility::Visible);
		}
		if (OnClickType == EDTButtonOnClickType::CheckButton)
		{
			MyButton->SetVisibility(ESlateVisibility::Visible);
		}
		ButtonSelectStyle();
		ButtonStatue = EDTButtonStatue::Select;
	}
	else
	{
		if (OnClickType != EDTButtonOnClickType::FreeButton)
		{
			FButtonStyle Style = MyButton->GetStyle();
			Style.SetNormal(ButtonStyle.Normal);
			MyButton->SetStyle(Style);
		}
		if (OnClickType == EDTButtonOnClickType::MutexButton)
		{
			MyButton->SetVisibility(ESlateVisibility::Visible);
		}
		if (OnClickType == EDTButtonOnClickType::CheckButton)
		{
			MyButton->SetVisibility(ESlateVisibility::Visible);
		}
		ButtonCommonStyle();
		ButtonStatue = EDTButtonStatue::Common;
	}
}

EDTButtonStatue UDTButtonBase::GetButtonStuate() const
{
	return ButtonStatue;
}

void UDTButtonBase::OnClickedCallBack()
{
	ButtonOnClikDelegate.ExecuteIfBound(ButtonIndex);
}

void UDTButtonBase::OnHoveredCallBack()
{
	ButtonHoveredStyle();
	ButtonStatue = EDTButtonStatue::Hovered;
}

void UDTButtonBase::OnUnHoveredCallBack()
{
	if (!IsSelect || OnClickType == EDTButtonOnClickType::FreeButton)
	{
		//ButtonCommonStyle();
		ButtonStatue = EDTButtonStatue::Common;
	}
}

void UDTButtonBase::OnPressCallBack()
{

}

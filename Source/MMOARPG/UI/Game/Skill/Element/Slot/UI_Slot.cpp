// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Slot.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUI_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	ClickButton->OnPressed.AddDynamic(this, &UUI_Slot::OnClickedWidget);
	ClickButton->OnReleased.AddDynamic(this, &UUI_Slot::OnReleasedClickedWidget);
}

void UUI_Slot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SlotBuild.Cooldown > 0.f)
	{
		SlotBuild.Cooldown -= InDeltaTime;
		DrawSlotCDMat(SlotBuild.Cooldown / SlotBuild.MaxCooldown);
		DrawSlotCDText(SlotBuild.Cooldown);

		ClearCD();
	}
}

void UUI_Slot::ResetSlot()
{
	Super::ResetSlot();

	SlotCDValue->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Slot::StartUpdateCD(float InCD)
{
	if (InCD > 0.f)
	{
		Super::StartUpdateCD(InCD);	
	}

	SlotBuild.Cooldown = InCD;
}

void UUI_Slot::StartUpdateCD()
{
	if (SlotBuild.Cooldown > 0.f)
	{
		Super::StartUpdateCD();
	}
}

void UUI_Slot::SetMaxCD(float InCD)
{
	SlotBuild.MaxCooldown = InCD;
}

void UUI_Slot::EndUpdateCD()
{
	Super::EndUpdateCD();
}

void UUI_Slot::ClearCD()
{
	if (SlotBuild.Cooldown <= 0.f)
	{
		FoceClearCD();
	}
}

void UUI_Slot::FoceClearCD()
{
	SlotBuild.Cooldown = 0.f;

	DrawSlotCDMat(0.f);
	DrawSlotCDText(0.f);

	EndUpdateCD();
}

void UUI_Slot::EnableClickButton(bool bEnable)
{
	ClickButton->SetIsEnabled(bEnable);
}

bool UUI_Slot::IsEnableClickButton()
{
	return ClickButton->bIsEnabled;
}

void UUI_Slot::SetKeyName(const FString& NewKeyName)
{
	SlotKey->SetText(FText::FromString(NewKeyName));
}

void UUI_Slot::DrawSlotCDText(float InSlotCD)
{
	DisplayNumber(SlotCDValue, InSlotCD);
}

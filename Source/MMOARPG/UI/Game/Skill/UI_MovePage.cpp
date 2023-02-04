// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_MovePage.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

FReply UUI_MovePage::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	if (HasMouseCapture())
	{
		if (UWidgetTree* InWidgetTree = Cast<UWidgetTree>(GetOuter()))
		{
			if (UWidget* InParentWidget = Cast<UWidget>(InWidgetTree->GetOuter()))
			{
				const FGeometry& InParentGeometry = InParentWidget->GetTickSpaceGeometry();
				const FVector2D& NewPostion = InMouseEvent.GetScreenSpacePosition();

				FVector2D NewOffsetA = InParentGeometry.AbsoluteToLocal(NewPostion) - Offset;
				FVector2D NewOffsetB = InParentGeometry.GetLocalSize() - GetDesiredSize();
				UCanvasPanelSlot* NewPanelSlot = Cast<UCanvasPanelSlot>(Slot);
				if (NewPanelSlot)
				{
					NewPanelSlot->SetPosition(FVector2D(FMath::Clamp(NewOffsetA.X, 0.f, NewOffsetB.X), FMath::Clamp(NewOffsetA.Y, 0.f, NewOffsetB.Y)));
					Reply.CaptureMouse(GetCachedWidget().ToSharedRef());
				}
			}
		}
	}

	return Reply;
}

FReply UUI_MovePage::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		Offset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		if (Offset.Y <= 80.f)
		{
			Reply.CaptureMouse(GetCachedWidget().ToSharedRef());
		}
		else
		{
			Reply.ReleaseMouseCapture();
		}
	}

	return Reply;
}

FReply UUI_MovePage::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	Reply.ReleaseMouseCapture();
	return Reply;
}

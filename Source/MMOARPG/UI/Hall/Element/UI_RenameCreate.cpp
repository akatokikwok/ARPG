// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RenameCreate.h"
#include "../UI_HallMain.h"
#include "../../../Core/Hall/HallPawn.h"

void UUI_RenameCreate::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = INDEX_NONE;
	FindNameButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::Callback_ClickedFindName);
	CreateButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::Callback_ClickedCreate);
	CancelButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::Callback_ClickedCancel);
}

void UUI_RenameCreate::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_RenameCreate::SetSlotPosition(const int32 InSlotPosition)
{

}

void UUI_RenameCreate::Callback_ClickedCreate()
{
	// 播Rename控件的淡出动画.
	if (UUI_HallMain* InMainPanel = GetParents<UUI_HallMain>()) {
		InMainPanel->PlayRenameOut();
		InMainPanel->ResetCharacterCreatePanel();// 还原Create面板.
	}
}

void UUI_RenameCreate::Callback_ClickedCancel()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>()) {
		InHall->PlayRenameOut();// 先把Rename面板淡出.
		InHall->ResetCharacterCreatePanel();// 还原Create面板.

// 		// 点取消按钮的时候也要把舞台人物从场景里移除掉.
// 		if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
// 			if (InPawn->CharacterStage != nullptr) {
// 				InPawn->CharacterStage->Destroy();
// 			}
// 		}
	}
}

void UUI_RenameCreate::Callback_ClickedFindName()
{

}

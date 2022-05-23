// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RenameCreate.h"
#include "../UI_HallMain.h"
#include "../../../Core/Hall/HallPawn.h"
#include "../../../Core/Hall/HallPlayerState.h"

void UUI_RenameCreate::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = INDEX_NONE;
	FindNameButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::Callback_ClickedFindName);
	CreateButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCreate_callback);
	CancelButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCancel_callback);
}

void UUI_RenameCreate::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_RenameCreate::SetSlotPosition(const int32 InSlotIndex)
{
	SlotPosition = InSlotIndex;
}

void UUI_RenameCreate::ClickedCreate_callback()
{
	// 播Rename控件的淡出动画.
	if (UUI_HallMain* InMainPanel = GetParents<UUI_HallMain>()) {
		if (AHallPlayerState* InPS = GetPlayerState<AHallPlayerState>()) {

			if (FMMOARPGCharacterAppearance* InCA = InPS->AddCharacterCA(SlotPosition)) {// 在PS里注册添加槽号为SlotPosition的CA存档成功.
				InCA->Name = EditableName->GetText().ToString();// 设置新存档的名字.
				InCA->Date = FDateTime::Now().ToString();// 设置新存档的日期.
				InCA->Lv = 1;// 新存档刚建立的时候设置为1.

				if (InCA->Name.IsEmpty() == true) {/* 玩家并没有键入名字*/
					InMainPanel->PrintLog(TEXT("Name that Player Typed cannot be Empty!"));
				}
				else {/* 玩家键入了名字*/
					InMainPanel->CreateCharacter(*InCA);// 向服务端发送创建角色请求.
				}

				InMainPanel->PlayRenameOut();// 播放Rename控件的淡出动画.
			}
		}
	}
}

void UUI_RenameCreate::ClickedCancel_callback()
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

/** 向服务端核验我们输入的名字. */
void UUI_RenameCreate::Callback_ClickedFindName()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>()) {
		FString CharName = EditableName->GetText().ToString();
		InHall->CheckRename(CharName);// 向服务端核验 输入的名字;
	}
}

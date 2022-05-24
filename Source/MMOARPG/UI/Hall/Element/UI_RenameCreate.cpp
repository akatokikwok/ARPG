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
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>()) {
		if (AHallPlayerState* InPS = GetPlayerState<AHallPlayerState>()) {
			FMMOARPGCharacterAppearance InCA;
			InCA.Name = EditableName->GetText().ToString();// 设置新存档的名字.
			InCA.Date = FDateTime::Now().ToString();// 设置新存档的日期.
			InCA.Lv = 1;// 新存档刚建立的时候设置为1.
			InCA.SlotPosition = SlotPosition;

			if (InCA.Name.IsEmpty() == true) {/* 玩家并没有键入名字*/
				InHall->PrintLog(TEXT("Name that Player Typed cannot be Empty!"));
			}
			else {/* 玩家键入了名字*/
				InHall->CreateCharacter(InCA);// 向服务端发送创建角色请求.
			}
		}
	}
}

void UUI_RenameCreate::ClickedCancel_callback()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>()) {
		// 手动删除舞台人物.
		if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
			if (InPawn->CharacterStage) {
				InPawn->CharacterStage->Destroy();
				InPawn->CharacterStage = nullptr;
			}
		}

		InHall->PlayRenameOut();// 先把Rename面板淡出.
		InHall->ResetCharacterCreatePanel();// 还原Create面板.
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

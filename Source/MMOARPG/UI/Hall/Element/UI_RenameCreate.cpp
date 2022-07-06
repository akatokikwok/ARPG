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
			if (auto* CurrentTmpCreateCharacter_CA = InPS->GetCurrentTmpCreateCharacterCA())
			{
				CurrentTmpCreateCharacter_CA->Name = EditableName->GetText().ToString();
				CurrentTmpCreateCharacter_CA->Date = FDateTime::Now().ToString();

				if (CurrentTmpCreateCharacter_CA->Name.IsEmpty() == true) {/* 玩家并没有键入名字*/
					InHall->PrintLog(TEXT("Name that Player Typed cannot be Empty!"));
				}
				else {/* 玩家键入了名字*/

					/// 向服务端发送创建角色请求.(这里传出去的由玩家填充的CA存档是临时编辑生成的CA存档.)
					InHall->CreateCharacter(*CurrentTmpCreateCharacter_CA);
				}
			}
		}
	}
}

void UUI_RenameCreate::ClickedCancel_callback()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>()) {
		if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>()) {
			if (FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotPosition)) {
				InHall->ResetCharacterAppearance(InCA);
			}
		}

		// 删除刚刚角色
		InHall->DestroyCharacter();

		InHall->PlayRenameOut();
		InHall->ResetCharacterCreatePanel();
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

void UUI_RenameCreate::SetEditableName(const FText& InName)
{
	EditableName->SetText(InName);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_CharacterButton.h"
#include "../../../Core/Hall/HallPawn.h"
#include "UI_CharacterCreatePanel.h"
#include "../UI_HallMain.h"

void UUI_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterButton->OnReleased.AddDynamic(this, &UUI_CharacterButton::Callback_ClickedCharacter);
	
	SlotPosition = INDEX_NONE;// 初始化槽位.
	CreateShowIcon->SetVisibility(ESlateVisibility::Visible);// 让加号可见.
	CharacterInfo->SetVisibility(ESlateVisibility::Collapsed);// 让用户信息背景槽坍缩.
}

void UUI_CharacterButton::NativeDestruct()
{
	Super::NativeDestruct();

}

/** 初始化按钮槽号 */
void UUI_CharacterButton::SetSlotPosition(const int32 InNewPos)
{
	SlotPosition = InNewPos;
}

/** 用于点击加号槽位被绑定的方法. */
void UUI_CharacterButton::Callback_ClickedCharacter()
{
	if (1) {
		// 生成1个舞台人物.
		if (CharacterStageClass != nullptr) {
			if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
				if (InPawn->CharacterStage != nullptr) {
					InPawn->CharacterStage->Destroy();// 若已有一个角色就删掉.
				}
				InPawn->CharacterStage = GetWorld()->SpawnActor<ACharacterStage>(CharacterStageClass, SpawnPoint, FRotator::ZeroRotator);

				if (InPawn->CharacterStage != nullptr) {

				}
			}
		}

		// 先拿到持有者层级的CreatePanel.即右侧边栏.
		if (UUI_CharacterCreatePanel* InUI_CharCreatePanel = GetParents<UUI_CharacterCreatePanel>()) {
			// 当点击加号后创建 捏脸信息.
			InUI_CharCreatePanel->CreateKneadFace();
			// 借助更上一层使用播UI动画的方法.
			if (UUI_HallMain* InMainPanel = InUI_CharCreatePanel->GetParents<UUI_HallMain>()) {
				InMainPanel->PlayRenameIn();// 播放Rename 面板的动画.
			}
		}
	}
}

/** 设置指定数据包的+号按钮外观. */
void UUI_CharacterButton::InitCharacterButton(const FMMOARPGCharacterAppearance& InCA)
{
	if (InCA.SlotPosition != INDEX_NONE) {
		CreateShowIcon->SetVisibility(ESlateVisibility::Collapsed);
		CharacterInfo->SetVisibility(ESlateVisibility::Visible);
		CharacterName->SetText(FText::FromString(InCA.Name));
 		Date->SetText(FText::FromString(InCA.Date));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_CharacterButton.h"
#include "../../../Core/Hall/HallPawn.h"
#include "UI_CharacterCreatePanel.h"
#include "../UI_HallMain.h"
#include "../../../Core/Hall/HallPlayerState.h"
#include "Kismet/GameplayStatics.h"

void UUI_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterButton->OnReleased.AddDynamic(this, &UUI_CharacterButton::ClickedCharacter_callback);

	SlotPosition = INDEX_NONE;// 初始化槽位.
	CreateShowIcon->SetVisibility(ESlateVisibility::Visible);// 让加号可见.
	CharacterInfo->SetVisibility(ESlateVisibility::Collapsed);// 让用户信息背景槽坍缩.
	DefaultColor = CharacterButton->WidgetStyle.Normal.TintColor.GetSpecifiedColor();// 初始化CharacterButton非高亮情况下的默认颜色.
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
void UUI_CharacterButton::ClickedCharacter_callback()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>()) {
		if (UUI_CharacterCreatePanel* UI_CharacterCreatePanel = GetParents<UUI_CharacterCreatePanel>()) {// 拿取父级Create面板.
			if (InState->IsCharacterExistInSlot(SlotPosition) == false) {// 查到PS里没有数据源
				
				// 每次生成舞台人物,注册一次槽号, 刷新一次身材外观.
				if (ACharacterStage* InStageChar = UI_CharacterCreatePanel->CreateCharacter()) {
					InStageChar->SetSlotID(SlotPosition);
					InStageChar->UpdateKneadingBoby();
				}

				// 当点击加号后创建 捏脸信息与控件的外观.
				UI_CharacterCreatePanel->CreateKneadFace();
				// 借助更上一级父级HallMain来播动画, 播Renmae面板的动画,同时产生Rename面板.
				if (UUI_HallMain* InMainPanel = UI_CharacterCreatePanel->GetParents<UUI_HallMain>()) {
					InMainPanel->PlayRenameIn();// 播放Rename 面板的动画.
					InMainPanel->SetSlotPosition(SlotPosition);// 设置Renmae 控件的关联槽号.
				}
			}
			else if (InState->IsCharacterExistInSlot(SlotPosition) == true) {// PS里找到了对应槽号数据就跳转关卡.

				// DS服务器
				UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameMap"));
			}

			// 每点击一次就要刷新一次槽号.
			UI_CharacterCreatePanel->SetCurrentSlotPosition(SlotPosition);// 记录一次当前槽号.(每点击一次就要刷新一次槽号.)
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

/** 使高亮 CharacterButton. */
void UUI_CharacterButton::SetHighlight(bool bHighlight)
{
	if (bHighlight) {
		CharacterButton->WidgetStyle.Normal.TintColor = HighlightColor;
		CharacterButton->SetStyle(CharacterButton->WidgetStyle);
	}
	else {
		CharacterButton->WidgetStyle.Normal.TintColor = DefaultColor;
		CharacterButton->SetStyle(CharacterButton->WidgetStyle);
	}
}

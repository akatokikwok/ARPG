// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_CharacterButton.h"

void UUI_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterButton->OnReleased.AddDynamic(this, &UUI_CharacterButton::Callback_ClickedCharacter);

	CreateShowIcon->SetVisibility(ESlateVisibility::Visible);// 让加号可见.
	CharacterInfo->SetVisibility(ESlateVisibility::Collapsed);// 让用户信息背景槽坍缩.
}

void UUI_CharacterButton::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_CharacterButton::Callback_ClickedCharacter()
{

}
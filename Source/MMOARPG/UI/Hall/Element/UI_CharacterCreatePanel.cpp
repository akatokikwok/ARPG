// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_CharacterCreatePanel.h"
#include "Components/ScrollBoxSlot.h"

void UUI_CharacterCreatePanel::NativeConstruct()
{
	Super::NativeConstruct();

	InitCharacterButton(4.0f);// 预先动态添加4个点击槽进去.
}

void UUI_CharacterCreatePanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterCreatePanel::InitCharacterButton(const int32 InNumber)
{
	if (UI_CharacterButtonClass != nullptr) {
		for (int32 i = 0; i < InNumber; ++i) {
			if (UUI_CharacterButton* InCharButton = CreateWidget<UUI_CharacterButton>(GetWorld(), UI_CharacterButtonClass)) {
				InCharButton->SetParents(this);// 此种Button的持有者被设定为this;
				if (UScrollBoxSlot* InScrollSlot = Cast<UScrollBoxSlot>(List->AddChild(InCharButton))) {// 往滑动框里添加元素.
					InScrollSlot->SetPadding(10.0f);// 设置被添加进的元素的间距为10.0f;
				}

			}
		}
	}
}

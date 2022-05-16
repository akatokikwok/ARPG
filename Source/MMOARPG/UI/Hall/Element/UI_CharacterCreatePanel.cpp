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

/**  负责 当点击加号后创建 捏脸信息的逻辑.*/
void UUI_CharacterCreatePanel::CreateKneadFace()
{
	// 1.先把滑动块里所有元素清掉.
	List->ClearChildren();
	// 2.创建并弹出1个捏脸控件并注册进滑动框.
	if (UI_KneadFaceClass != nullptr) {
		if (UUI_KneadFace* InUIKnead = CreateWidget<UUI_KneadFace>(GetWorld(), UI_KneadFaceClass)) {
			if (UScrollBoxSlot* InScrollSlot = Cast<UScrollBoxSlot>(List->AddChild(InUIKnead))) {
				InScrollSlot->SetPadding(10.0f);// 设置被添加进的元素的间距为10.0f;
			}
		}
	}
}

void UUI_CharacterCreatePanel::CreateCharacterButtons()
{
	InitCharacterButton(4.0f);
}

void UUI_CharacterCreatePanel::InitCharacterButton(const int32 InNumber)
{
	List->ClearChildren();// 先清空所有元素.

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

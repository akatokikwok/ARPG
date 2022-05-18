﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_CharacterCreatePanel.h"
#include "Components/ScrollBoxSlot.h"
#include "../../../Core/Hall/HallPlayerState.h"
#include "UI_CharacterButton.h"
#include "../../../Core/Hall/HallPawn.h"

void UUI_CharacterCreatePanel::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = 0;// 设定当前选中的玩家人物槽号为0号;
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

/** 创建所有带数据的加号外观.(角色数据来源是PS) */
void UUI_CharacterCreatePanel::CreateCharacterButtons()
{
	// 拿取PS里的角色形象数据初始化一堆加号.
	if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>()) {
		InitCharacterButtons(InPlayerState->GetCharacterAppearance());
	}
}

/** 初始化所有关联数据包的 加号外观.(默认设定是4个) */
void UUI_CharacterCreatePanel::InitCharacterButtons(const FCharacterAppearances& InCAs)
{
	List->ClearChildren();// 先清空所有元素.

	if (UI_CharacterButtonClass != nullptr) {
		for (int32 i = 0; i < 4.0f; ++i) {// 默认设定为4个加号按钮.
			if (UUI_CharacterButton* InCharButton = CreateWidget<UUI_CharacterButton>(GetWorld(), UI_CharacterButtonClass)) {
				/** 滑动框里添加并注册+号按钮实体. */
				if (UScrollBoxSlot* InScrollSlot = Cast<UScrollBoxSlot>(List->AddChild(InCharButton))) {// 往滑动框里添加元素.
					InScrollSlot->SetPadding(10.0f);// 设置被添加进的元素的间距为10.0f;
				}

				/* 给创建出的button设定槽号*/
				InCharButton->SetSlotPosition(i);
				InCharButton->SetParents(this);

				/** 初始化各个数据包关联的+号按钮外观. */
				if (const FMMOARPGCharacterAppearance* InCharacterAppearance =
					InCAs.FindByPredicate([&](const FMMOARPGCharacterAppearance& InCharacterAppearance) ->bool {
						return InCharacterAppearance.SlotPosition == i;
					})) {
					InCharButton->InitCharacterButton(*InCharacterAppearance);// 设置指定数据包 的+号按钮外观.
				}
			}
		}
	}
}
/** 生成指定槽号的玩家形象 */
void UUI_CharacterCreatePanel::SpawnCharacter(const int32 InSlotIndex)
{
	if (AHallPlayerState* InPS = GetPlayerState<AHallPlayerState>()) {
		SpawnCharacter(
			// 满足 在PS里查找到 指定槽号的数据包.
			InPS->GetCharacterAppearance().FindByPredicate([&](const FMMOARPGCharacterAppearance& InCA) ->bool {
				return InCA.SlotPosition == InSlotIndex;
			})
		);
	}

// 		if (FMMOARPGCharacterAppearance* InCAData = InPS->GetCharacterAppearance().FindByPredicate([&](const FMMOARPGCharacterAppearance& InCA) ->bool {
// 				return InCA.SlotPosition == InSlotIndex;
// 			}))
// 		{
// 			SpawnCharacter(InCAData);
// 		}
}

/** 仅负责生成玩家人物. */
void UUI_CharacterCreatePanel::SpawnCharacter()
{
	SpawnCharacter(SlotPosition);
}

/** 生成关联特定CA的玩家形象 */
void UUI_CharacterCreatePanel::SpawnCharacter(const FMMOARPGCharacterAppearance* InACData)
{
	if (InACData != nullptr) {
		// 生成1个舞台人物.
		if (CharacterStageClass != nullptr) {
			if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
				if (InPawn->CharacterStage != nullptr) {
					InPawn->CharacterStage->Destroy();// 若已有一个角色就删掉.
				}
				InPawn->CharacterStage = GetWorld()->SpawnActor<ACharacterStage>(CharacterStageClass, SpawnPoint, FRotator::ZeroRotator);// 生成1个新的人物.

				if (InPawn->CharacterStage != nullptr) {

				}
			}
		}
	}
}

/**  */
void UUI_CharacterCreatePanel::SetCurrentSlotPosition(const int32 InNewPos)
{
	SlotPosition = InNewPos;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_CharacterButton.h"
#include "../../../Core/Hall/HallPawn.h"

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
	if (CharacterStageClass != nullptr) {
		if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
			if (InPawn->CharacterStage != nullptr) {
				InPawn->CharacterStage->Destroy();// 若已有一个角色就删掉.
			}
			// 生成1个舞台人物.
			InPawn->CharacterStage = GetWorld()->SpawnActor<ACharacterStage>(CharacterStageClass, SpawnPoint, FRotator::ZeroRotator);
			
			if (InPawn->CharacterStage != nullptr) {

			}
		}

		
	}
}
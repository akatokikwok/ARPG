// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MMOARPGType.h"
#include "HallPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	FCharacterAppearances& GetCharacterAppearance();
	FMMOARPGCharacterAppearance* GetRecentCharacter();
	FMMOARPGCharacterAppearance* GetCurrentTmpCreateCharacter() { return &CurrentTmpCreateCharacter; }

private:
	FCharacterAppearances CharacterAppearances;// 一堆角色形象.
	FMMOARPGCharacterAppearance CurrentTmpCreateCharacter;// 单个玩家形象.
};

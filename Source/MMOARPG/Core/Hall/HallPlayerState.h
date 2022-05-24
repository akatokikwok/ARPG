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
	// 拿取一堆角色形象.
	FCharacterAppearances& GetCharacterAppearance();
	/** 检查所有舞台角色是否存在符合指定槽位的数据包 */
	bool IsCharacterExistInSlot(const int32 InPos);
	// 获取最近游玩过的存档角色的数据.
	FMMOARPGCharacterAppearance* GetRecentCharacter();
	// 拿取指定槽号的CA存档.
	FMMOARPGCharacterAppearance* GetCharacterCA(const int32 InPosIndex);
	// 添加指定槽号的CA(若已有则拿取已有的.)
	FMMOARPGCharacterAppearance* AddCharacterCA(const int32 InPosIndex);
	//
	int32 AddCharacterCA(const FMMOARPGCharacterAppearance& InCA);
	
	//
	FMMOARPGCharacterAppearance* GetCurrentTmpCreateCharacter() 
	{ 
		return &CurrentTmpCreateCharacter; 
	}

private:
	FCharacterAppearances CharacterAppearances;// 一堆角色形象.
	FMMOARPGCharacterAppearance CurrentTmpCreateCharacter;// 单个玩家形象.
};

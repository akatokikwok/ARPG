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

	// 移除指定槽号的CA存档.
	void RemoveCharacterAppearanceBySlot(int32 InSlot);

	// 获取最近游玩过的存档角色的数据.
	FMMOARPGCharacterAppearance* GetRecentCharacter();
	// 拿取指定槽号的CA存档.
	FMMOARPGCharacterAppearance* GetCharacterCA(const int32 InPosIndex);
	// 添加指定槽号的CA(若已有则拿取已有的.)
	FMMOARPGCharacterAppearance* AddCharacterCA(const int32 InPosIndex);
	//
	int32 AddCharacterCA(const FMMOARPGCharacterAppearance& InCA);
	
	// 拿取临时生成的CA存档.
	FMMOARPGCharacterAppearance* GetCurrentTmpCreateCharacterCA() 
	{ 
		return &CurrentTmpCreateCharacter_CA; 
	}

private:
	FCharacterAppearances CharacterAppearances;// 一堆角色形象(和服务端交互).

	// 临时生成的单个人物数据(例如在UI里拖拉滑条的时候临时生成).
	FMMOARPGCharacterAppearance CurrentTmpCreateCharacter_CA;
};

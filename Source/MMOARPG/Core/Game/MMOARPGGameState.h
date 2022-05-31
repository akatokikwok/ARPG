// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../../DataTable/CharacterAnimTable.h"
#include "MMOARPGGameState.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AMMOARPGGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMMOARPGGameState();

	/** 从DT里找出指定ID的 蒙太奇动画数据.(可能返空) */
	FCharacterAnimTable* GetCharacterAnimTable(int32 InAnimTableID);
	// 拿到DT里所有数据存到 数据包集合里.
	TArray<FCharacterAnimTable*>* GetCharacterAnimTables();

protected:
	// 管理多行数据的动画DataTable.
	UPROPERTY()
		UDataTable* CharacterAnimTablePtr;
	
	// 动画数据包集合.
	TArray<FCharacterAnimTable*> CharacterAnimTables;
};

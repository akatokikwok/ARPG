// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGGameState.h"

AMMOARPGGameState::AMMOARPGGameState()
{
	// 使用FObjectFinder 找到CharacterAnimTable这个资产.
	// 设置本类的动画DataTable.
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimTable(TEXT("/Game/DataTable/CharacterAnimTable"));
	CharacterAnimTablePtr = CharacterAnimTable.Object;
}

FCharacterAnimTable* AMMOARPGGameState::GetCharacterAnimTable(int32 InAnimTableID)
{
	if (TArray<FCharacterAnimTable*>* AnimTables = GetCharacterAnimTables()) {// 获取DataTable里所有数据源(很多个结构体的数组).

		if (AnimTables->Num()) {
			if (auto Anim = AnimTables->FindByPredicate([&](FCharacterAnimTable* InAnimTable) ->bool {
				return InAnimTable->ID == InAnimTableID;
				}))
			{
				return *Anim;
			}
		}
	}

	return NULL;
}

TArray<FCharacterAnimTable*>* AMMOARPGGameState::GetCharacterAnimTables()
{
	if (!CharacterAnimTables.Num()) {
		// 获取DataTable里所有的数据源.
		if (CharacterAnimTablePtr != nullptr) {
			CharacterAnimTablePtr->GetAllRows(TEXT("AnimTable"), CharacterAnimTables);
		}
	}

	return &CharacterAnimTables;
}
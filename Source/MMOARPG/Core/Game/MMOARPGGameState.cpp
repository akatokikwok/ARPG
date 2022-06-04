// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGGameState.h"

AMMOARPGGameState::AMMOARPGGameState()
{
	// 使用FObjectFinder 找到CharacterAnimTable这个资产.
	// 设置本类的动画DataTable.
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimTable(TEXT("/Game/DataTable/CharacterAnimTable"));
	CharacterAnimTablePtr = CharacterAnimTable.Object;

	// 同上, 加载角色样式DT 蓝图资源.
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStyleTable(TEXT("/Game/DataTable/CharacterStyleTable"));
	CharacterStyleTablePtr = CharacterStyleTable.Object;
}

FCharacterAnimTable* AMMOARPGGameState::GetCharacterAnimTable(int32 InAnimTableID)
{
	// 从动画DT里 读出指定ID的那一行
	return GetTable_read(InAnimTableID, CharacterAnimTablePtr, CharacterAnimTables, TEXT("AnimTable"));
}

TArray<FCharacterAnimTable*>* AMMOARPGGameState::GetCharacterAnimTables()
{
	// 把已存的多行蒙太奇动画数据 写入一张动画DT里.
	return GetTables_write(CharacterAnimTablePtr, CharacterAnimTables, TEXT("AnimTable"));
}

FCharacterStyleTable* AMMOARPGGameState::GetCharacterStyleTable(int32 InCharacterTableID)
{
	return GetTable_read(InCharacterTableID, CharacterStyleTablePtr, CharacterStyleTables, TEXT("CharacterTable"));
}

TArray<FCharacterStyleTable*>* AMMOARPGGameState::GetCharacterStyleTables()
{
	return GetTables_write(CharacterStyleTablePtr, CharacterStyleTables, TEXT("CharacterTable"));
}

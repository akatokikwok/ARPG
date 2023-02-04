// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGGameState.h"
#include "Abilities/GameplayAbility.h"

AMMOARPGGameState::AMMOARPGGameState()
{
	// 使用FObjectFinder 找到CharacterAnimTable这个资产.
	// 设置本类的动画DataTable.
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimTable(TEXT("/Game/DataTable/CharacterAnimTable"));
	CharacterAnimTablePtr = CharacterAnimTable.Object;

	// 同上, 加载角色样式DT 蓝图资源.
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStyleTable(TEXT("/Game/DataTable/CharacterStyleTable"));
	CharacterStyleTablePtr = CharacterStyleTable.Object;

	// 同上, 加载角色样式DT 蓝图资源.
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterSkillTable(TEXT("/Game/DataTable/CharacterSkillTable"));
	CharacterSkillTablePtr = CharacterSkillTable.Object;

	// 同上, 加载属性集DT 蓝图资源.
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAttributeTable(TEXT("/Game/DataTable/CharacterAttributeTable"));
	CharacterAttributeTablePtr = CharacterAttributeTable.Object;
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

FCharacterSkillTable* AMMOARPGGameState::GetCharacterSkillTable(const FName& InSkillKey, int32 InCharacterSkillTableID)
{
	if (TArray<FCharacterSkillTable*>* AllRows = GetTables_write(CharacterSkillTablePtr, CharacterSkillTables, TEXT("SkillTable"))) {
		/** FindByPredicate条件匹配 */
		if (FCharacterSkillTable** InFoundSkillTable = AllRows->FindByPredicate(
			[&](const FCharacterSkillTable* InSKillData)->bool {
				if (InSKillData->ID == InCharacterSkillTableID) {
					return InSKillData->GameplayAbility.GetDefaultObject()->AbilityTags == FGameplayTagContainer(FGameplayTag::RequestGameplayTag(InSkillKey));
				}
				return false;
			})
			) {
			return *InFoundSkillTable;
		}
	}

	return NULL;
}

TArray<FCharacterSkillTable*>* AMMOARPGGameState::GetCharacterSkillTables()
{
	return GetTables_write(CharacterSkillTablePtr, CharacterSkillTables, TEXT("SkillTable"));
}

FCharacterAttributeTable* AMMOARPGGameState::GetCharacterAttributeTable(int32 InCharacterTableID)
{
	return GetTable_read(InCharacterTableID, CharacterAttributeTablePtr, CharacterAttributeTables, TEXT("CharacterAttributeTable"));
}

TArray<FCharacterAttributeTable*>* AMMOARPGGameState::GetCharacterAttributeTables()
{
	return GetTables_write(CharacterAttributeTablePtr, CharacterAttributeTables, TEXT("CharacterAttribute"));
}

/** 提取出给定ID号的人物的所有行技能信息(即某个ID的人物拥有多少个技能) */
bool AMMOARPGGameState::GetCharacterSkillsTables(int32 InCharacterID, TArray<FCharacterSkillTable*>& OutSkillTables)
{
	if (TArray<FCharacterSkillTable*>* InCharacterSkillTables = GetCharacterSkillTables()) {// 先拿取所有行技能信息.
		if (FCharacterSkillTable** FoundedElePtr = InCharacterSkillTables->FindByPredicate(// 若确保能查找到 给定人物ID的那行技能信息
				[InCharacterID](const FCharacterSkillTable* InSingleRowPtr) ->bool { 
					return InSingleRowPtr->ID == InCharacterID; 
				})) {
				
			// 再提取到出参
			for (auto& Row_Skill : *InCharacterSkillTables) {
				if (Row_Skill->ID == InCharacterID) {
					OutSkillTables.Add(Row_Skill);
				}
			}
			return OutSkillTables.Num() > 0;
		}
	}
	return false;
}

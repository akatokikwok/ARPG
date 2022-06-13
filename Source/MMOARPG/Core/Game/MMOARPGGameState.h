// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../../DataTable/CharacterAnimTable.h"
#include "../../DataTable/CharacterStyleTable.h"
#include "../../DataTable/CharacterSkillTable.h"
#include "MMOARPGGameState.generated.h"

/**
 * GS,管理DataTable之类的数据.
 */
UCLASS()
class MMOARPG_API AMMOARPGGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMMOARPGGameState();

	FCharacterAnimTable* GetCharacterAnimTable(int32 InAnimTableID);// 从DT里读指定ID的动画行
	TArray<FCharacterAnimTable*>* GetCharacterAnimTables();// 把多行动画数据写进动画DT.

	FCharacterStyleTable* GetCharacterStyleTable(int32 InCharacterTableID);// 从DT里读指定ID的 人物样式行
	TArray<FCharacterStyleTable*>* GetCharacterStyleTables();// 把多行人物样式 写入样式DT.

	FCharacterSkillTable* GetCharacterSkillTable(int32 InSkillTableID);// 从DT里读指定ID的 技能
	TArray<FCharacterSkillTable*>* GetCharacterSkillTables();// 把多行技能 写入技能DT.

protected:/// 一些关于DT读写行数据的通用模板.

	/* 把多行DTRow 写入一张蓝图DT里.*/
	template<class T>
	TArray<T*>* GetTables_write(UDataTable* InTable, TArray<T*>& rowDatas, const FString& MsgTag = TEXT("MyTable"))
	{
		if (!rowDatas.Num()) {
			if (InTable) {
				InTable->GetAllRows(MsgTag, rowDatas);
			}
		}

		return &rowDatas;
	}

	/* 在指定DT里 读出 指定ID号 的单行 .*/
	template<class T>
	T* GetTable_read(int32 InTableRow_ID, UDataTable* OneTable, TArray<T*>& Rows, const FString& MsgTag = TEXT("MyTable"))
	{
		if (TArray<T*>* AllRows_res = this->GetTables_write<T>(OneTable, Rows, MsgTag)) {
			if (AllRows_res->Num()) {
				if (auto InTable = AllRows_res->FindByPredicate([&](T* InSingleRow) {return InSingleRow->ID == InTableRow_ID; })) {
					return *InTable;
				}
			}
		}

		return NULL;
	}


protected:
	// 管理多行数据的动画 的DataTable.
	UPROPERTY()
		UDataTable* CharacterAnimTablePtr;
	// 管理多行角色样式 的DataTable.
	UPROPERTY()
		UDataTable* CharacterStyleTablePtr;

	// 管理多行技能 的DataTable.
	UDataTable* CharacterSkillTablePtr;

	// 动画蒙太奇 DTRow集合.
	TArray<FCharacterAnimTable*> CharacterAnimTables;
	// 角色样式 DTRow集合.
	TArray<FCharacterStyleTable*> CharacterStyleTables;
	// 技能 DTRow集合.
	TArray<FCharacterSkillTable*> CharacterSkillTables;
};

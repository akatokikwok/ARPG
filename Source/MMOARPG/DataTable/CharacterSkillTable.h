#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "CharacterSkillTable.generated.h"

class UGameplayAbility;

/**
 * 与DataTable搭配使用(本质上是DT的一行).
 * 用以管理技能的结构体.
 */
USTRUCT(BlueprintType)
struct FCharacterSkillTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterSkillTable();
public:
	TSubclassOf<UGameplayAbility>* FindComboAttack(const FName& InKey);// 用Key获取缓存池:combo形式
	TSubclassOf<UGameplayAbility>* FindSkillAttack(const FName& InKey);// 用Key获取缓存池:skill形式

public:
	// ID号, 存在于负责技能的DTRow.
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		int32 ID;
	
	// 普攻(连招)形式的攻击 缓存池, 存在于负责技能的DTRow;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		TArray<TSubclassOf<UGameplayAbility>> ComboAttack;// 攻击缓存池:连招形式, 存在于负责技能的DTRow;
	
	// 技能形式的攻击 缓存池, 存在于负责技能的DTRow;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		TArray<TSubclassOf<UGameplayAbility>> SkillAttack;// 攻击缓存池:技能形式, 存在于负责技能的DTRow;
};
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Abilities/GameplayAbility.h"
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

	// ID号, 存在于负责技能的DTRow.
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		int32 ID;
	
	// 普攻(连招)形式的攻击 缓存池, 存在于负责技能的DTRow;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		TMap<FName, TSubclassOf<UGameplayAbility>> ComboAttack;// 普攻(连招)形式的攻击 缓存池, 存在于负责技能的DTRow;
	
	// 技能形式的攻击 缓存池, 存在于负责技能的DTRow;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		TMap<FName, TSubclassOf<UGameplayAbility>> SkillAttack;// 技能形式的攻击 缓存池, 存在于负责技能的DTRow;
};
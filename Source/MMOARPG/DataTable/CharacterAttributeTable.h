﻿
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "CharacterAttributeTable.generated.h"

/**
 * 任意生命体(具备生命)的属性表.(本质上是DT的一行).
 * 比如血、蓝、物理伤害、魔法伤害、护甲、魔抗
 * 技能标签集合、连击标签集合、肢体动作标签集合
 */
USTRUCT(BlueprintType)
struct FCharacterAttributeTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterAttributeTable();

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		int32 ID;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		int32 Level;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float Health;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float Mana;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float PhysicsAttack;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float MagicAttack;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float MagicDefense;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float PhysicsDefense;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float MaxEmpiricalValue;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float DamageBase;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float ExperienceReward;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float StaminaValue;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float MaxStaminaValue;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float HealthRate;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float ManaRate;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		float StaminaRate;

	// 人物固定天生拥有的Skill形式技能
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		TArray<FGameplayTag> SkillTags;

	// 人物固定天生拥有的Combo连打形式技能
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		TArray<FGameplayTag> ComboAttackTags;

	// 人物固定天生拥有的Limb肢体形式技能
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		TArray<FGameplayTag> LimbsTags;
};
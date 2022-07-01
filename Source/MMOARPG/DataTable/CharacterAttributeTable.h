
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
		TArray<FGameplayTag> SkillTags;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		TArray<FGameplayTag> ComboAttackTags;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAttribute")
		TArray<FGameplayTag> LimbsTags;
};
#include "CharacterAttributeTable.h"
#include "Abilities/GameplayAbility.h"

FCharacterAttributeTable::FCharacterAttributeTable()
	: ID(INDEX_NONE)
	, Level(1)
	, Health(100.f)
	, Mana(50.f)
	, PhysicsAttack(52.f)
	, MagicAttack(0.f)
	, MagicDefense(0.f)
	, PhysicsDefense(20.f)
	, AttackRange(200.f)
	, MaxEmpiricalValue(100.f)
	, DamageBase(15)
	, ExperienceReward(30)
	, StaminaValue(100.f)
	, MaxStaminaValue(100.f)
{
	// 任意生命体都具备以下的技能与行为.

	ComboAttackTags.Add(FGameplayTag::RequestGameplayTag(TEXT("Player.Attack.ComboLinkage")));
	LimbsTags.Add(FGameplayTag::RequestGameplayTag(TEXT("Player.State.Die")));
	LimbsTags.Add(FGameplayTag::RequestGameplayTag(TEXT("Player.State.Hit")));
}
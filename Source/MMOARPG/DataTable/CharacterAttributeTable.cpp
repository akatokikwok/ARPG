#include "CharacterAttributeTable.h"
#include "Abilities/GameplayAbility.h"

FCharacterAttributeTable::FCharacterAttributeTable()
	:ID(INDEX_NONE)
	, Health(100.f)
	, Mana(50.f)
	, PhysicsAttack(52.f)
	, MagicAttack(0.f)
	, MagicDefense(0.f)
	, PhysicsDefense(20.f)
	, AttackRange(200.f)
{
	ComboAttackTags.Add(FGameplayTag::RequestGameplayTag(TEXT("Player.Attack.ComboLinkage")));
	LimbsTags.Add(FGameplayTag::RequestGameplayTag(TEXT("Player.State.Die")));
	LimbsTags.Add(FGameplayTag::RequestGameplayTag(TEXT("Player.State.Hit")));
}
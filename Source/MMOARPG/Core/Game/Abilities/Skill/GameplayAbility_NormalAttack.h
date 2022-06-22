#pragma once

#include "CoreMinimal.h"
#include "../MMOARPGGameplayAbility.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "Core/GameplayAbility_Skill.h"
#include "GameplayAbility_NormalAttack.generated.h"

/**
 * GA:普攻
 */
UCLASS()
class MMOARPG_API UGameplayAbility_NormalAttack : public UGameplayAbility_Skill
{
	GENERATED_BODY()
public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};

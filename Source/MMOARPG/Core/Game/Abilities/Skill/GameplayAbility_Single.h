#pragma once

#include "CoreMinimal.h"
#include "../MMOARPGGameplayAbility.h"
#include "Core/GameplayAbility_Skill.h"
#include "GameplayAbility_Single.generated.h"

/**
 * GA: 单技能
 */
UCLASS()
class MMOARPG_API UGameplayAbility_Single : public UGameplayAbility_Skill
{
	GENERATED_BODY()
public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};

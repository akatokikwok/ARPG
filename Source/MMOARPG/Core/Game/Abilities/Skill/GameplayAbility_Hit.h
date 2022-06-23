#pragma once

#include "CoreMinimal.h"
#include "Core/GameplayAbility_Skill.h"
#include "GameplayAbility_Hit.generated.h"

/**
 * GA: 挨打受击.
 * 继承自GA_Skill
 */
UCLASS()
class MMOARPG_API UGameplayAbility_Hit : public UGameplayAbility_Skill
{
	GENERATED_BODY()
public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};

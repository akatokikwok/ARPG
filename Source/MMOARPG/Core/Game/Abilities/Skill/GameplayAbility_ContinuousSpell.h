// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_Single.h"
#include "GameplayAbility_ContinuousSpell.generated.h"

/**
 * 技能:持续消耗
 */
UCLASS()
class MMOARPG_API UGameplayAbility_ContinuousSpell : public UGameplayAbility_Single
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameplayAbility_Skill.h"
#include "GameplayAbility_Random.generated.h"

/**
 * GA: 随机播放蒙太奇的任一段section
 */
UCLASS()
class MMOARPG_API UGameplayAbility_Random : public UGameplayAbility_Skill
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
};

// Fill out your copyright notice in the Description page of Project Settings.
#include "GameplayAbility_ContinuousSpell.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "../../../../MMOARPGGameType.h"

void UGameplayAbility_ContinuousSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 注册消耗
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor)) {
		if (FContinuousReleaseSpell* InReleaseSpell = InCharacterBase->GetContinuousReleaseSpell()) {
			InReleaseSpell->BuffPtr = GetCostGameplayEffect();
		}
	}
}
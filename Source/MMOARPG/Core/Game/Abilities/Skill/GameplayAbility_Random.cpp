// Fill out your copyright notice in the Description page of Project Settings.
#include "GameplayAbility_Random.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

void UGameplayAbility_Random::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

// 	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
// 		return;
// 	}

	if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true)) {// 强制不提交CD
		return;
	}

	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor)) {
		if (UMMOARPGGameplayAbility::PlayMontageAnim(*FString::FromInt(FMath::RandRange(0, GetCompositeSectionsNumber() - 1)))) {

		}
	}
}
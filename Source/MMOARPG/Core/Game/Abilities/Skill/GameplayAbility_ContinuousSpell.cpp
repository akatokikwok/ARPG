// Fill out your copyright notice in the Description page of Project Settings.
#include "GameplayAbility_ContinuousSpell.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "../../../../MMOARPGGameType.h"

void UGameplayAbility_ContinuousSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 当发动持续施法GA的时候, 要注册黑盒里的消耗
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor)) {
		if (FContinuousReleaseSpell* InReleaseSpell = InCharacterBase->GetContinuousReleaseSpell()) {
			// 设定黑盒内的消耗buff为这张GA里配置的cost buff
			InReleaseSpell->BuffPtrObj = this->GetCostGameplayEffect();
		}
	}
}
﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_NormalAttack.h"

void UGameplayAbility_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		return;// 技能提交失败则直接放弃.
	}
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor)) {
		/* 传入明确的 第几段招式*/
		if (UMMOARPGGameplayAbility::PlayMontageAnim(
				*FString::FromInt(InCharacterBase->GetSimpleComboInfo()->ComboIndex) )) {
			//
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, FString::SanitizeFloat(InCharacterBase->GetSimpleComboInfo()->ComboIndex));
		}
	}
}

void UGameplayAbility_NormalAttack::OnCompleted()
{
	Super::OnCompleted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_NormalAttack::OnBlendOut()
{
	Super::OnBlendOut();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_NormalAttack::OnInterrupted()
{
	Super::OnInterrupted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_NormalAttack::OnCancelled()
{
	Super::OnCancelled();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

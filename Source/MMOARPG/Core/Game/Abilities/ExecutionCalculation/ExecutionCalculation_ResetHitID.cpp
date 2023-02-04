// Fill out your copyright notice in the Description page of Project Settings.
#include "ExecutionCalculation_ResetHitID.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "AbilitySystemComponent.h"

UExecutionCalculation_ResetHitID::UExecutionCalculation_ResetHitID()
	: HitIDByGround(INDEX_NONE)
	, HitIDByAir(INDEX_NONE)
	, bAuthorityPlayHit(true)
{

}

void UExecutionCalculation_ResetHitID::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	// 拿到自己的和敌人的ASC
	UAbilitySystemComponent* TargetAC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetAC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(TargetAC->GetOwner())) {
			if (!InCharacter->IsDie()) {
				bool bPlay = false;
				if (InCharacter->GetFightComponent()->IsAir()) {
					if (HitIDByAir != INDEX_NONE) {
						InCharacter->SetHitID(HitIDByAir);
						bPlay = true;
					}
				}
				else {
					if (HitIDByGround != INDEX_NONE) {
						InCharacter->SetHitID(HitIDByGround);
						bPlay = true;
					}
				}

				// 放受击技能
				if (bAuthorityPlayHit && bPlay) {
					InCharacter->PlayHit();
				}
			}
		}
	}
}
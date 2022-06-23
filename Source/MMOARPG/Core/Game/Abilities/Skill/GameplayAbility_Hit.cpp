#include "GameplayAbility_Hit.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

void UGameplayAbility_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		return;// 技能提交失败则直接放弃.
	}

	/**  */
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor)) {
		if (UMMOARPGGameplayAbility::PlayMontageAnim(*FString::FromInt(InCharacterBase->GetHitID()))) {// 播放第几个section? 由战斗组件的受击ID决定.
			
		}
	}
}

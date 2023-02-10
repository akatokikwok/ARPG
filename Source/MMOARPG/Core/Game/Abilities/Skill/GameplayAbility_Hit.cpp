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
			// 必须蒙太奇播放成功,才会将本技能注册进活跃标签组
			UMMOARPGGameplayAbility::RegisterActiveSkillTag();

			// 提交这个条件分型的技能
			UMMOARPGGameplayAbility::CommitAbilityConditionalSkills(Handle, ActorInfo, ActivationInfo);
		}
	}
}

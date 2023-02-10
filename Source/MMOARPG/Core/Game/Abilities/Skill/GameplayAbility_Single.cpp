#include "GameplayAbility_Single.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

void UGameplayAbility_Single::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
// 	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
// 		return;// 技能提交失败则直接放弃.
// 	}
	
	if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true)) {// 强制不提交CD
		return;
	}
	
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor)) {
		/* 因为是单技能,所以只传入并播首个section. */
		if (UMMOARPGGameplayAbility::PlayMontageAnim(*FString::FromInt(0.f))) {
			// 必须蒙太奇播放成功,才会将本技能注册进活跃标签组
			UMMOARPGGameplayAbility::RegisterActiveSkillTag();

			// 通知客户端更新CD
			UMMOARPGGameplayAbility::CallUpdateCooldownOnClient();

			// 提交这个条件分型的技能
			UMMOARPGGameplayAbility::CommitAbilityConditionalSkills(Handle, ActorInfo, ActivationInfo);
		}
	}
}

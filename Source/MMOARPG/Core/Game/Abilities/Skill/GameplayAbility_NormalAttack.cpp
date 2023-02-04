#include "GameplayAbility_NormalAttack.h"

void UGameplayAbility_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		return;// 技能提交失败则直接放弃.
	}
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor)) {
		/* 传入明确的 第几段招式*/
		if (UMMOARPGGameplayAbility::PlayMontageAnim(*FString::FromInt(InCharacterBase->GetSimpleComboInfo(*UGameplayAbility::AbilityTags.ToStringSimple())->ComboIndex) )) {
			
			// 仅打印提示表明当前是第几段Combo.
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, FString::Printf(TEXT("这是第 %d 段combo招式."), InCharacterBase->GetSimpleComboInfo()->ComboIndex));

			// 通知客户端更新CD
			//UMMOARPGGameplayAbility::CallUpdateCooldownOnClient();
		}
	}
}

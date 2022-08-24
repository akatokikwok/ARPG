#include "MMOARPGAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "../Character/Core/MMOARPGCharacterBase.h"

UMMOARPGAbilitySystemComponent::UMMOARPGAbilitySystemComponent()
{

}

UAbilitySystemComponent* UMMOARPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent /*= false*/)
{
	return Cast<UMMOARPGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

/**
 * 覆写基类的,摘抄原函数逻辑过来进行修改,以便于项目适配
 * 以下代码均更改自基类的 UAbilitySystemComponent.PlayMontage 
 */
float UMMOARPGAbilitySystemComponent::PlayMontage(UGameplayAbility* InAnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName /*= NAME_None*/, float StartTimeSeconds /*= 0.0f*/)
{
	if (AbilityActorInfo.IsValid() && NewAnimMontage) {
		if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(AbilityActorInfo->OwnerActor)) {
			if (IsOwnerActorAuthoritative()) {
				// 服务器上 广播蒙太奇动画.
				InCharacterBase->MontagePlayOnMulticast(NewAnimMontage, InPlayRate, StartSectionName);
			}
			else {
				// 客户端上 RPC服务器广播蒙太奇动画.
				InCharacterBase->MontagePlayOnServer(NewAnimMontage, InPlayRate, StartSectionName);
			}

			// 返回蒙太奇长度.
			return NewAnimMontage->GetPlayLength();
		}
	}

	return INDEX_NONE;
}

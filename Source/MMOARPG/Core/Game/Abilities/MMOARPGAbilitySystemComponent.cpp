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
				InCharacterBase->MontagePlayOnMulticast(NewAnimMontage, InPlayRate, StartTimeSeconds, true, StartSectionName);
			}
			else {
				// 客户端上 RPC服务器广播蒙太奇动画.
				InCharacterBase->MontagePlayOnServer(NewAnimMontage, InPlayRate, StartTimeSeconds, true, StartSectionName);
			}

			// 手动注册一个当前的蒙太奇
			LocalAnimMontageInfo.AnimMontage = NewAnimMontage;

			// 返回蒙太奇长度.
			return NewAnimMontage->GetPlayLength();
		}
	}

	return INDEX_NONE;
}

// 检查已激活的活跃Buff里是否匹配给定标签
bool UMMOARPGAbilitySystemComponent::IsActiveGameplayEffectTags(const FGameplayTag& InTag)
{
	// 拿到所有已激活GE的handle
	TArray<FActiveGameplayEffectHandle> GAHandle = UAbilitySystemComponent::ActiveGameplayEffects.GetAllActiveEffectHandles();
	// 扫描任意已激活的handle(实际上会转化为扫描GE)
	for (auto& Tmp : GAHandle) {
		if (FActiveGameplayEffect* InActiveGameplayEffect = ActiveGameplayEffects.GetActiveGameplayEffect(Tmp)) {
			if (const UGameplayEffect* InDef = InActiveGameplayEffect->Spec.Def) {
				// 查被激活的活跃GE资产标签是否匹配给定标签
				if (InDef->InheritableGameplayEffectTags.CombinedTags.HasTag(InTag)) {
					return true;
				}
			}
		}
	}
	return false;
}

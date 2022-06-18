#include "MMOARPGGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask/AbilityTask_PMAWDamageEvent.h"

UMMOARPGGameplayAbility::UMMOARPGGameplayAbility()
{

}

void UMMOARPGGameplayAbility::OnCompleted()
{
	K2_OnCompleted();
}

void UMMOARPGGameplayAbility::OnBlendOut()
{
	K2_OnBlendOut();
}

void UMMOARPGGameplayAbility::OnInterrupted()
{
	K2_OnInterrupted();
}

void UMMOARPGGameplayAbility::OnCancelled()
{
	K2_OnCancelled();
}

/** 拿取本GA里指定蒙太奇里的总段数. */
int32 UMMOARPGGameplayAbility::GetCompositeSectionsNumber()
{
	if (MontageToPlay) {
		return MontageToPlay->CompositeSections.Num();
	}
	return 0;
}

/** 仿UAbilityTask_PlayMontageAndWait创建静态节点并绑定代理. */
UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::CreatePlayMontageAndWaitProxy(/*UGameplayAbility* OwningAbility, */FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/, float StartTimeSeconds /*= 0.f*/)
{
	// 构建1个AT节点; <UAbilityTask_PNAWDamageEvent>型;
	UAbilityTask_PNAWDamageEvent* InWait_Node =
		UAbilityTask_PNAWDamageEvent::CreatePNAWDamageEventProxy(this, TaskInstanceName, InMontageToPlay, AbilityTags, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale, StartTimeSeconds);
	
	if (InWait_Node != nullptr) {
		InWait_Node->OnCompleted.AddDynamic(this, &UMMOARPGGameplayAbility::OnCompleted);
		InWait_Node->OnBlendOut.AddDynamic(this, &UMMOARPGGameplayAbility::OnBlendOut);
		InWait_Node->OnInterrupted.AddDynamic(this, &UMMOARPGGameplayAbility::OnInterrupted);
		InWait_Node->OnCancelled.AddDynamic(this, &UMMOARPGGameplayAbility::OnCancelled);
		InWait_Node->DamageEventReceived.AddDynamic(this, &UMMOARPGGameplayAbility::OnDamageGameplayEvent);// 受击委托也要绑定回调.


 		InWait_Node->Activate();// 激活ATNode
		//InWait_Node->ReadyForActivation();
		return InWait_Node;
	}

	return nullptr;
}

/** 播蒙太奇的哪个section */
UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::PlayMontageAnim(FName StartSection /*= NAME_None*/)
{
	return CreatePlayMontageAndWaitProxy(NAME_None, MontageToPlay, 1.f, StartSection);
}

// <UAbilityTask_PNAWDamageEvent>节点 受击委托绑定的回调.
// 虚方法,允许派生类的GA覆写
void UMMOARPGGameplayAbility::OnDamageGameplayEvent(FGameplayTag InGameplayTag, FGameplayEventData Payload)
{

}


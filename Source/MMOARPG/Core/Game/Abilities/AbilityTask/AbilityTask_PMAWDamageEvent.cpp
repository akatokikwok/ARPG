﻿#include "AbilityTask_PMAWDamageEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

// 覆写显式构造.
UAbilityTask_PNAWDamageEvent::UAbilityTask_PNAWDamageEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

// 覆写AbilityTask的激活.
void UAbilityTask_PNAWDamageEvent::Activate()
{
	if (Ability == nullptr) {/* GA合理性判断.*/
		return;
	}

	if (AbilitySystemComponent) {/* 仿自基类Activate部分的逻辑.*/
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		// 使用ASC::AddGameplayEventTagContainerDelegate 去接收外部给到的GATag 并存成一个句柄.
		if (AnimInstance != nullptr) {
			DamageEventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(
				EventTags,
				FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAbilityTask_PNAWDamageEvent::OnDamageGameplayEvent)
			);
		}
	}

	Super::Activate();
}

// 覆写AbilityTask的删除或卸载.
void UAbilityTask_PNAWDamageEvent::OnDestroy(bool AbilityEnded)
{
	// 移除标签组和施击伤害句柄.
	if (AbilitySystemComponent) {
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, DamageEventHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

// AT节点创建.
UAbilityTask_PNAWDamageEvent* UAbilityTask_PNAWDamageEvent::CreatePNAWDamageEventProxy(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	UAnimMontage* MontageToPlay,
	FGameplayTagContainer InEventTags,
	float Rate, FName StartSection,
	bool bStopWhenAbilityEnds,
	float AnimRootMotionTranslationScale,
	float StartTimeSeconds)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UAbilityTask_PNAWDamageEvent* MyObj = NewAbilityTask<UAbilityTask_PNAWDamageEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = StartTimeSeconds;
	MyObj->EventTags = InEventTags;// 标签组也得注册.

	return MyObj;
}

// 回调; 当伤害事件发生时 动态多播由持有者GA传入的标签与伤害event
void UAbilityTask_PNAWDamageEvent::OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData* Payload)
{
	if (UAbilityTask::ShouldBroadcastAbilityTaskDelegates()) {
		FGameplayEventData EventData = *Payload;
		EventData.EventTag = InGameplayTag;

		DamageEventReceived.Broadcast(InGameplayTag, EventData);// 多播由持有者GA传入的标签与伤害event.
	}
}
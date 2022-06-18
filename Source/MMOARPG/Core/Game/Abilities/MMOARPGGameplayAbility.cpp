﻿#include "MMOARPGGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask/AbilityTask_PMAWDamageEvent.h"
#include "AbilitySystemComponent.h"

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
// 接收BoxHit那边SendGameplayEventToActor这个API传过来的伤害事件与标签组
// 虚方法,允许派生类的GA覆写
void UMMOARPGGameplayAbility::OnDamageGameplayEvent(FGameplayTag InGameplayTag, FGameplayEventData Payload)
{
	if (FMMOARPGGameplayEffects* InEffect = EffectMap.Find(InGameplayTag)) {// 以传入标签为依据, 查找缓存池里的 GE群租.
		FMMOARPGGameplayEffectSpec GameplayEffectSpec_Pak;// 待注册的"GE包"
		///  待注册的"GE包"填充逻辑 与 施击和受击具体注册逻辑与编排顺序.
		{
			// new一个受击的伤害承受者; 使用GAS框架里推荐的 FGameplayAbilityTargetData_ActorArray
			FGameplayAbilityTargetData_ActorArray* NewTargetData_ActorArray = new FGameplayAbilityTargetData_ActorArray();
			// 用入参-伤害事件去填充 受击者的字段.
			NewTargetData_ActorArray->TargetActorArray.Add(const_cast<AActor*>(Payload.Target));
			// 再用写入完成的受击者去填充 单个GE的TargetHandleData.
			GameplayEffectSpec_Pak.TargetHandleData.Add(NewTargetData_ActorArray);

			/* 扫描待注册的单个GE包里持有的所有GE句柄.*/
			for (auto& Tmp : InEffect->TargetEffectClasses) {
				// 先使用当前正在播的GA的数据源 构建1个 UE-- GE实例句柄.
				FGameplayEffectSpecHandle NewHandle_GE =
					GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(Tmp, 1, MakeEffectContext(CurrentSpecHandle, CurrentActorInfo));
				//
				if (NewHandle_GE.IsValid()) {
					// 再拿取当前释放的GA实例.
					FGameplayAbilitySpec* AbilitySpec =
						GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromHandle(CurrentSpecHandle);
					// 把挂在GA实例上的Tag赠送一份给GE实例
					ApplyAbilityTagsToGameplayEffectSpec(*NewHandle_GE.Data.Get(), AbilitySpec);
					// 做值结束后填充GE包.
					if (AbilitySpec) {
						NewHandle_GE.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;// 用GA的标签量级填充GE的标签量级
						GameplayEffectSpec_Pak.TargetEffectSpecs.Add(NewHandle_GE);// 填充GE包.
					}
				}
			}
			//
		}

		/* 应用提取的每个GE句柄至GE作用目标*/
		for (auto& Tmp : GameplayEffectSpec_Pak.TargetEffectSpecs) {
			TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles = K2_ApplyGameplayEffectSpecToTarget(Tmp, GameplayEffectSpec_Pak.TargetHandleData);
		}
	}
}

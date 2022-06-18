#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_PMAWDamageEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMontageWaitSimpleForDamageEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);
/**
 * 一个AbilityTask,用于计算伤害.
 * 派生自UE源码的UAbilityTask_PlayMontageAndWait.
 * Ability task to simply play a montage. Many games will want to make a modified version of this task that looks for game-specific events.
 */
UCLASS()
class MMOARPG_API UAbilityTask_PNAWDamageEvent : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()
public:
	// 多播代理: 当受击
	UPROPERTY(BlueprintAssignable)
		FMontageWaitSimpleForDamageEventDelegate DamageEventReceived;// 多播代理: 当受击

public:
	UAbilityTask_PNAWDamageEvent(const FObjectInitializer& ObjectInitializer);// 覆写显式构造.
	virtual void Activate() override;// 覆写AbilityTask的激活.
	virtual void OnDestroy(bool AbilityEnded) override;// 覆写AbilityTask的删除或卸载.
	
	// AT节点创建.
	static UAbilityTask_PNAWDamageEvent* CreatePNAWDamageEventProxy(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		FGameplayTagContainer InEventTags,
		float Rate = 1.f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float AnimRootMotionTranslationScale = 1.f,
		float StartTimeSeconds = 0.f);
private:
	// 回调; 当伤害事件发生时 动态多播由持有者GA传入的标签与伤害event
	void OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData* Payload);

private:
	UPROPERTY()
		FGameplayTagContainer EventTags;

private:
	FDelegateHandle DamageEventHandle;// 用以接收受击结算代理的句柄.
};
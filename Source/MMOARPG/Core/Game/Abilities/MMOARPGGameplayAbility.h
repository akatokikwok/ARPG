#pragma once

#include "Abilities/GameplayAbility.h"
#include "MMOARPGGameplayAbilityType.h"
#include "MMOARPGGameplayAbility.generated.h"
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class MMOARPG_API UMMOARPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UMMOARPGGameplayAbility();

public:// Cpp版. 调用蓝图版的
	UFUNCTION()
		virtual void OnCompleted();
	UFUNCTION()
		virtual void OnBlendOut();
	UFUNCTION()
		virtual void OnInterrupted();
	UFUNCTION()
		virtual void OnCancelled();
public:// 蓝图版. 提供给CPP版使用
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnCompleted", meta = (ScriptName = "OnCompleted"))
		void K2_OnCompleted();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnBlendOut", meta = (ScriptName = "OnBlendOut"))
		void K2_OnBlendOut();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnInterrupted", meta = (ScriptName = "OnInterrupted"))
		void K2_OnInterrupted();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnCancelled", meta = (ScriptName = "OnCancelled"))
		void K2_OnCancelled();
public:
	// 拿取蒙太奇内部的总段数.
	int32 GetCompositeSectionsNumber();

	// 读取出buff在特定等级和在特定属性下的耗费值
	float CostValue(const FString& InCostName, float InLevel);

public:
	/** 仿UAbilityTask_PlayMontageAndWait创建静态节点并绑定代理. */
	UFUNCTION(BlueprintCallable, Category = "MMOARPGGameplayAbility|Tasks", meta = (DisplayName = "PlayMontageAndWait"/*, HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE")*/))
		UAbilityTask_PlayMontageAndWait* CreatePlayMontageAndWaitProxy(/*UGameplayAbility* OwningAbility, */FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

	// 播蒙太奇的哪个section.
	UFUNCTION(BlueprintCallable, Category = "MMOARPGGameplayAbility|Tasks")
		UAbilityTask_PlayMontageAndWait* PlayMontageAnim(FName StartSection = NAME_None);

	// <UAbilityTask_PNAWDamageEvent>节点 受击委托绑定的回调.
	// 虚方法,允许派生类的GA覆写
	UFUNCTION()
		virtual	void OnDamageGameplayEvent(FGameplayTag InGameplayTag, FGameplayEventData Payload);

protected:
	// 在客户端更新CD
	void CallUpdateCooldownOnClient();

/// //////////////////////////////////////////////////////////////////////////
public:
	// 播哪个蒙太奇.
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
		UAnimMontage* MontageToPlay;

	// 缓存池: 由一个GA诱发带来的一组buff.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
		TMap<FGameplayTag, FMMOARPGGameplayEffects> EffectMap;

public:
	// 条件分型技能的 条件标签组
	UPROPERTY(EditDefaultsOnly, Category = "Tags", meta = (Categories = "OwnedTagsCategory"))
		FGameplayTagContainer ConditionalActivationTags;// 条件型技能的 条件标签组
};
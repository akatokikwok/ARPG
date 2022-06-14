#pragma once

#include "Abilities/GameplayAbility.h"
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
	/** 仿UAbilityTask_PlayMontageAndWait创建静态节点并绑定代理. */
	UFUNCTION(BlueprintCallable, Category = "MMOARPGGameplayAbility|Tasks", meta = (DisplayName = "PlayMontageAndWait"/*, HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE")*/))
		UAbilityTask_PlayMontageAndWait* CreatePlayMontageAndWaitProxy(/*UGameplayAbility* OwningAbility, */FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

	// 播蒙太奇的哪个section.
	UFUNCTION(BlueprintCallable, Category = "MMOARPGGameplayAbility|Tasks")
		UAbilityTask_PlayMontageAndWait* PlayMontageAnim(FName StartSection = NAME_None);

/// //////////////////////////////////////////////////////////////////////////
public:
	// 播哪个蒙太奇.
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
		UAnimMontage* MontageToPlay;
};
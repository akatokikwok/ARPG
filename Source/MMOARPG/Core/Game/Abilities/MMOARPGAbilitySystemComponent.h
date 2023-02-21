#pragma once

#include "AbilitySystemComponent.h"
#include "MMOARPGAbilitySystemComponent.generated.h"

UCLASS()
class MMOARPG_API UMMOARPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UMMOARPGAbilitySystemComponent();

	/** Searches the passed in actor for an ability system component, will use the AbilitySystemInterface */
	static UAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	/** Plays a montage and handles replication and prediction based on passed in ability/activation info */
	virtual float PlayMontage(UGameplayAbility* AnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName = NAME_None, float StartTimeSeconds = 0.0f);

public:
	// 设定当前游戏中 "活跃的GAS标签"
	void SetCurrentActiveSkillTags(FGameplayTagContainer& InGameplayTagContainer) { CurrentActiveSkillTags = InGameplayTagContainer; }

	// 获取到 当前游戏中 "活跃的GAS标签"
	const FGameplayTagContainer* GetCurrentActiveSkillTags()const { return &CurrentActiveSkillTags; }

	// 复位 当前游戏中 "活跃的GAS标签"
	void ResetCurrentActiveSkillTags() { CurrentActiveSkillTags.Reset(); }

	// 检查已激活的活跃Buff里是否匹配给定标签
	bool IsActiveGameplayEffectTags(const FGameplayTag& InTag);

protected:
	// 当前游戏进行中活跃的GAS标签
	UPROPERTY()
		FGameplayTagContainer CurrentActiveSkillTags;// 当前游戏进行中活跃的GAS标签
};
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
};
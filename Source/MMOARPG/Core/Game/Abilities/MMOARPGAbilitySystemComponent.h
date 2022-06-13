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

};
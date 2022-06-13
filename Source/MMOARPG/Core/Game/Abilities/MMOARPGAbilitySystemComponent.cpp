#include "MMOARPGAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UMMOARPGAbilitySystemComponent::UMMOARPGAbilitySystemComponent()
{

}

UAbilitySystemComponent* UMMOARPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent /*= false*/)
{
	return Cast<UMMOARPGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

#include "MMOARPGAttributeSet.h"

UMMOARPGAttributeSet::UMMOARPGAttributeSet()
{

}

// 覆写AttributeSet的PostGameplayEffectExecute接口.
void UMMOARPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}


#pragma once

#include "AttributeSet.h"
#include "MMOARPGAttributeSet.generated.h"

UCLASS()
class MMOARPG_API UMMOARPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMMOARPGAttributeSet();
	// 覆写AttributeSet的PostGameplayEffectExecute接口.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};
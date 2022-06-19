#pragma once

#include "MMOARPGGameplayAbilityMacros.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "MMOARPGType.h"
#include "MMOARPGAttributeSet.generated.h"

UCLASS()
class MMOARPG_API UMMOARPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMMOARPGAttributeSet();
public:
	// 属性同步; 属性在服务器发生变化时，对应的客户端自动调用OnRep函数.
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Health)

	//
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MaxHealth)

	//
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
		FGameplayAttributeData Mana;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Mana)

	//
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
		FGameplayAttributeData MaxMana;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MaxMana)

/// //////////////////////////////////////////////////////////////////////////
public:
	// 覆写AttributeSet的PostGameplayEffectExecute接口.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 覆写 同步变量需要重写的方法.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
};
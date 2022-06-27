#pragma once

#include "MMOARPGGameplayAbilityMacros.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "MMOARPGType.h"
#include "MMOARPGAttributeSet.generated.h"

/**
 * MMO项目属性集
 */
UCLASS()
class MMOARPG_API UMMOARPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMMOARPGAttributeSet();
public:
	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_Level)
		FGameplayAttributeData Level;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Level)

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

	// 属性: 伤害
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Damage)
		FGameplayAttributeData Damage;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Damage)

	// 物理杀伤值
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_PhysicsAttack)
		FGameplayAttributeData PhysicsAttack;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, PhysicsAttack)

	// 魔法杀伤值
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MagicAttack)
		FGameplayAttributeData MagicAttack;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MagicAttack)

	// 护甲
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_PhysicsDefense)
		FGameplayAttributeData PhysicsDefense;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, PhysicsDefense)

	// 魔抗
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MagicDefense)
		FGameplayAttributeData MagicDefense;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MagicDefense)

	// 攻击范围
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_AttackRange)
		FGameplayAttributeData AttackRange;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, AttackRange)

/// //////////////////////////////////////////////////////////////////////////
public:
	// 覆写AttributeSet的PostGameplayEffectExecute接口.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 覆写 同步变量需要重写的方法.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 注册GAS属性集
	virtual void RegistrationProperties(const FMMOARPGCharacterAttribute& Data);
protected:
	UFUNCTION()
		virtual void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_PhysicsAttack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MagicAttack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_PhysicsDefense(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MagicDefense(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_AttackRange(const FGameplayAttributeData& OldValue);
protected:
	// 工具方法:
	void RegistrationParam(FGameplayAttributeData& InAttributeData, const FMMOARPGAttributeData& InNewAttributeData);
};
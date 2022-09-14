#pragma once

#include "MMOARPGGameplayAbilityMacros.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "MMOARPGType.h"
#include "MMOARPGAttributeSet.generated.h"
struct FCharacterAttributeTable;

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
	// 覆写接口: PostGameplayEffectExecute.(属性被修改之后)
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 覆写接口: PreGameplayEffectExecute.(属性未被修改之前)
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	// 覆写 同步变量需要重写的方法.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 用服务器端的GAS属性集 注册本AS类里各字段.
	virtual void RegistrationProperties(const FMMOARPGCharacterAttribute& Data);

	// 用DTR_属性 注册本AS类里各字段.
	virtual void RegistrationProperties(const FCharacterAttributeTable* Data);

public:
	/** 把AS里所有属性字段 从UEAD型转化为MMOAD型. */
	virtual void ToMMOARPGCharacterAttribute(FMMOARPGCharacterAttribute& OutData);

protected:
	// Rep等级
	UFUNCTION()
		virtual void OnRep_Level(const FGameplayAttributeData& OldValue);
	// Rep血
	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
	// Rep最大血
	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	// Rep蓝
	UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);
	// Rep最大蓝
	UFUNCTION()
		virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
	// Rep伤害值
	UFUNCTION()
		virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);
	// Rep物伤
	UFUNCTION()
		virtual void OnRep_PhysicsAttack(const FGameplayAttributeData& OldValue);
	// Rep法伤
	UFUNCTION()
		virtual void OnRep_MagicAttack(const FGameplayAttributeData& OldValue);
	// Rep护甲
	UFUNCTION()
		virtual void OnRep_PhysicsDefense(const FGameplayAttributeData& OldValue);
	// Rep魔抗
	UFUNCTION()
		virtual void OnRep_MagicDefense(const FGameplayAttributeData& OldValue);
	// Rep攻击距离
	UFUNCTION()
		virtual void OnRep_AttackRange(const FGameplayAttributeData& OldValue);
	// Rep经验
	UFUNCTION()
		virtual void OnRep_EmpiricalValue(const FGameplayAttributeData& OldValue);
	// Rep最大经验
	UFUNCTION()
		virtual void OnRep_MaxEmpiricalValue(const FGameplayAttributeData& OldValue);

protected:
	// 工具方法:
	void RegistrationParam(FGameplayAttributeData& InAttributeData, const FMMOARPGAttributeData& InNewAttributeData);
	// 工具方法.
	void RegistrationParam(FGameplayAttributeData& InAttributeData, const float InValue);
	// 把一个UEAD(AttributeData)转化为1个MMOAD
	void RegistrationParam(FMMOARPGAttributeData& InNewAttributeData, const FGameplayAttributeData& InAttributeData);

public:
	// 属性同步; 属性在服务器发生变化时，对应的客户端自动调用OnRep函数.
	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_Level)
		FGameplayAttributeData Level;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Level)

		//
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Health)

		//
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MaxHealth)

		//
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Mana)
		FGameplayAttributeData Mana;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Mana)

		//
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxMana)
		FGameplayAttributeData MaxMana;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MaxMana)

		// 属性: 伤害
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Damage)
		FGameplayAttributeData Damage;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Damage)

		// 物理杀伤值
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_PhysicsAttack)
		FGameplayAttributeData PhysicsAttack;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, PhysicsAttack)

		// 魔法杀伤值
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MagicAttack)
		FGameplayAttributeData MagicAttack;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MagicAttack)

		// 护甲
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_PhysicsDefense)
		FGameplayAttributeData PhysicsDefense;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, PhysicsDefense)

		// 魔抗
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MagicDefense)
		FGameplayAttributeData MagicDefense;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MagicDefense)

		// 攻击范围
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_AttackRange)
		FGameplayAttributeData AttackRange;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, AttackRange)

		// 经验值
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_EmpiricalValue)
		FGameplayAttributeData EmpiricalValue;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, EmpiricalValue)

		// 最大经验值
		UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxEmpiricalValue)
		FGameplayAttributeData MaxEmpiricalValue;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MaxEmpiricalValue)
};
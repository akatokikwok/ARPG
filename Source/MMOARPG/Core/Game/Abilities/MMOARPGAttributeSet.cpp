#include "MMOARPGAttributeSet.h"

UMMOARPGAttributeSet::UMMOARPGAttributeSet()
	: Health(100.f)
	, MaxHealth(100.f)
	, Mana(0.f)
	, MaxMana(0.f)
{

}

// 覆写AttributeSet的PostGameplayEffectExecute接口.
void UMMOARPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}

// 覆写 同步变量需要重写的方法.
void UMMOARPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMMOARPGAttributeSet, Health);
	DOREPLIFETIME(UMMOARPGAttributeSet, MaxHealth);
	DOREPLIFETIME(UMMOARPGAttributeSet, Mana);
	DOREPLIFETIME(UMMOARPGAttributeSet, MaxMana);
}

// 注册GAS属性集
void UMMOARPGAttributeSet::RegistrationProperties(const FMMOARPGCharacterAttribute& Data)
{
	this->RegistrationParam(Health, Data.Health);
	this->RegistrationParam(MaxHealth, Data.MaxHealth);
	this->RegistrationParam(Mana, Data.Mana);
	this->RegistrationParam(MaxMana, Data.MaxMana);
}

void UMMOARPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, Health, OldValue);
}

void UMMOARPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, MaxHealth, OldValue);
}

void UMMOARPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, Mana, OldValue);
}

void UMMOARPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, MaxMana, OldValue);
}

// 仅工具方法.
void UMMOARPGAttributeSet::RegistrationParam(FGameplayAttributeData& InAttributeData, const FMMOARPGAttributeData& InNewAttributeData)
{
	InAttributeData.SetBaseValue(InNewAttributeData.BaseValue);
	InAttributeData.SetCurrentValue(InNewAttributeData.CurrentValue);
}


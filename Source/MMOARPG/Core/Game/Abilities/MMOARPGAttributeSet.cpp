#include "MMOARPGAttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectExtension.h"
#include "../Character/Core/MMOARPGCharacterBase.h"

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
	// 解算出Tag容器.
	const FGameplayTagContainer& SourceTagContainer = *(Data.EffectSpec.CapturedSourceTags.GetAggregatedTags());
	// 解算出buff作用目标人物.
	AMMOARPGCharacterBase* Target = 
		Data.Target.AbilityActorInfo.IsValid() ? Cast<AMMOARPGCharacterBase>(Data.Target.AbilityActorInfo->AvatarActor) : nullptr;

	float Magnitude = 0.f;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive) {
		Magnitude = Data.EvaluatedData.Magnitude;
	}

	// 若是属性: Health
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));// 设定血
		// 通知目标人去操作血量
		if (Target != nullptr) {
			Target->HandleHealth(SourceTagContainer, Magnitude);
		}
	}
	// 若是属性: Mana
	else if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
		// 通知目标人去操作蓝量
		if (Target != nullptr) {
			Target->HandleMana(SourceTagContainer, Magnitude);
		}
	}

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


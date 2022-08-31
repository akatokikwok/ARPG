#include "MMOARPGAttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectExtension.h"
#include "../Character/Core/MMOARPGCharacterBase.h"
#include "../../../DataTable/CharacterAttributeTable.h"

UMMOARPGAttributeSet::UMMOARPGAttributeSet()
	: Level(1)
	, Health(200.f)
	, MaxHealth(200.f)
	, Mana(0.f)
	, MaxMana(0.f)
	, Damage(0.f)
	, PhysicsAttack(51.f)
	, MagicAttack(0.f)
	, MagicDefense(0.f)
	, PhysicsDefense(20.f)
	, AttackRange(200.f)
	, EmpiricalValue(0.f)
	, MaxEmpiricalValue(100.f)
{

}

// 覆写AttributeSet的PostGameplayEffectExecute接口.
void UMMOARPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();// Buff上下文.
	UAbilitySystemComponent* SourceAbilitySystemComponent = Context.GetOriginalInstigatorAbilitySystemComponent();// 源ASC.

	// 解算出Tag容器.
	const FGameplayTagContainer& SourceTagContainer = *(Data.EffectSpec.CapturedSourceTags.GetAggregatedTags());
	// 解算出buff作用目标人物.
	AMMOARPGCharacterBase* Target =
		Data.Target.AbilityActorInfo.IsValid() ? Cast<AMMOARPGCharacterBase>(Data.Target.AbilityActorInfo->AvatarActor) : nullptr;

	float Magnitude = 0.f;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive) {
		Magnitude = Data.EvaluatedData.Magnitude;
	}

	/* Lambda--负责加工源Char和源Actor.*/
	auto GetSourceCharacterAndSourceActor_Lambda = [&](AActor*& InSourceActor, AMMOARPGCharacterBase*& InSourceCharacter) {
		if (Target) {
			// 借助源ASC拿取 源Actor和源Controller 并转化为适用于人形态的.
			InSourceActor = SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
			AController* SourceController = SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && InSourceActor != nullptr) {
				if (APawn* InPawn = Cast<APawn>(InSourceActor)) {
					SourceController = InPawn->GetController();
				}
			}

			// 判定施法者到底是 MMOARPG人物形态还是一个源actor形态
			if (SourceController) {
				InSourceCharacter = Cast<AMMOARPGCharacterBase>(SourceController->GetPawn());
			}
			else {
				InSourceCharacter = Cast<AMMOARPGCharacterBase>(InSourceActor);
			}
		}
	};

	/* 关于属性: 血条*/
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));// 设定血
		
		if (Target != nullptr) {
			// 先用lambda加工源Char和源Actor
			AMMOARPGCharacterBase* SourceCharacter = nullptr;
			AActor* SourceActor = nullptr;
			GetSourceCharacterAndSourceActor_Lambda(SourceActor, SourceCharacter);
			// 操作血量处理.
			Target->HandleHealth(SourceCharacter, SourceActor, SourceTagContainer, Magnitude);
		}
	}
	/* 关于属性: 蓝条*/
	else if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));// 设定蓝
		// 通知目标人去操作蓝量
		if (Target != nullptr) {
			Target->HandleMana(SourceTagContainer, Magnitude);
		}
	}
	/* 若是属性: 伤害*/
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {
		const float TmpDamage = GetDamage();// 记录一下当前临时伤害值.
		SetDamage(0.f);// 清零.

		const float OldHealth = GetHealth();// 记录被攻击前的血量.
		SetHealth(FMath::Clamp(OldHealth - TmpDamage, 0.f, GetMaxHealth()));// 设定受击后的残余血量

		const float OldHealth11 = GetHealth();

		if (Target) {
			// 先用lambda加工源Char和源Actor
			AMMOARPGCharacterBase* SourceCharacter = nullptr;
			AActor* SourceActor = nullptr;
			GetSourceCharacterAndSourceActor_Lambda(SourceActor, SourceCharacter);

			// 命令目标去操作伤害和血量处理.
			Target->HandleDamage(TmpDamage, SourceTagContainer, SourceCharacter, SourceActor);
			Target->HandleHealth(SourceCharacter, SourceActor, SourceTagContainer, -TmpDamage);
		}
	}
}

// 覆写 同步变量需要重写的方法.
void UMMOARPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 使用 DOREPLIFETIME宏来同步这些字段.
	DOREPLIFETIME(UMMOARPGAttributeSet, Health);
	DOREPLIFETIME(UMMOARPGAttributeSet, MaxHealth);
	DOREPLIFETIME(UMMOARPGAttributeSet, Mana);
	DOREPLIFETIME(UMMOARPGAttributeSet, MaxMana);
	DOREPLIFETIME(UMMOARPGAttributeSet, PhysicsAttack);
	DOREPLIFETIME(UMMOARPGAttributeSet, MagicAttack);
	DOREPLIFETIME(UMMOARPGAttributeSet, MagicDefense);
	DOREPLIFETIME(UMMOARPGAttributeSet, AttackRange);
	DOREPLIFETIME(UMMOARPGAttributeSet, PhysicsDefense);
	DOREPLIFETIME(UMMOARPGAttributeSet, EmpiricalValue);
	DOREPLIFETIME(UMMOARPGAttributeSet, MaxEmpiricalValue);
}

// 用服务器端的GAS属性集 注册本AS类里各字段.
void UMMOARPGAttributeSet::RegistrationProperties(const FMMOARPGCharacterAttribute& Data)
{
	RegistrationParam(Level, Data.Level);
	RegistrationParam(Health, Data.Health);
	RegistrationParam(MaxHealth, Data.MaxHealth);
	RegistrationParam(Mana, Data.Mana);
	RegistrationParam(MaxMana, Data.MaxMana);
	RegistrationParam(PhysicsAttack, Data.PhysicsAttack);
	RegistrationParam(MagicAttack, Data.MagicAttack);
	RegistrationParam(PhysicsDefense, Data.PhysicsDefense);
	RegistrationParam(MagicDefense, Data.MagicDefense);
	RegistrationParam(AttackRange, Data.AttackRange);
	RegistrationParam(EmpiricalValue, Data.EmpiricalValue);
	RegistrationParam(MaxEmpiricalValue, Data.MaxEmpiricalValue);
}

// 用DTR_属性 注册本AS类里各字段.
void UMMOARPGAttributeSet::RegistrationProperties(const FCharacterAttributeTable* Data)
{
	RegistrationParam(Level, 1);
	RegistrationParam(Health, Data->Health);
	RegistrationParam(MaxHealth, Data->Health);
	RegistrationParam(Mana, Data->Mana);
	RegistrationParam(MaxMana, Data->Mana);
	RegistrationParam(PhysicsAttack, Data->PhysicsAttack);
	RegistrationParam(MagicAttack, Data->MagicAttack);
	RegistrationParam(PhysicsDefense, Data->PhysicsDefense);
	RegistrationParam(MagicDefense, Data->MagicDefense);
	RegistrationParam(AttackRange, Data->AttackRange);
	RegistrationParam(MaxEmpiricalValue, Data->MaxEmpiricalValue);
}

void UMMOARPGAttributeSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, Level, OldValue);
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

void UMMOARPGAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, Damage, OldValue);
}

void UMMOARPGAttributeSet::OnRep_PhysicsAttack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, PhysicsAttack, OldValue);
}

void UMMOARPGAttributeSet::OnRep_MagicAttack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, MagicAttack, OldValue);
}

void UMMOARPGAttributeSet::OnRep_PhysicsDefense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, PhysicsDefense, OldValue);
}

void UMMOARPGAttributeSet::OnRep_MagicDefense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, MagicDefense, OldValue);
}

void UMMOARPGAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, AttackRange, OldValue);
}

void UMMOARPGAttributeSet::OnRep_EmpiricalValue(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, EmpiricalValue, OldValue);
}

void UMMOARPGAttributeSet::OnRep_MaxEmpiricalValue(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, MaxEmpiricalValue, OldValue);
}

// 仅工具方法.
void UMMOARPGAttributeSet::RegistrationParam(FGameplayAttributeData& InAttributeData, const FMMOARPGAttributeData& InNewAttributeData)
{
	InAttributeData.SetBaseValue(InNewAttributeData.BaseValue);
	InAttributeData.SetCurrentValue(InNewAttributeData.CurrentValue);
}

// 工具方法
void UMMOARPGAttributeSet::RegistrationParam(FGameplayAttributeData& InAttributeData, const float InValue)
{
	InAttributeData.SetBaseValue(InValue);
	InAttributeData.SetCurrentValue(InValue);
}


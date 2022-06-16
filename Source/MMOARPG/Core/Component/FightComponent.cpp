// Fill out your copyright notice in the Description page of Project Settings.


#include "FightComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ThreadManage.h"
#include "Net/UnrealNetwork.h"
#include "../Game/MMOARPGGameState.h"
#include "../Game/Abilities/MMOARPGGameplayAbility.h"

UFightComponent::UFightComponent()
{

}

void UFightComponent::BeginPlay()
{
	Super::BeginPlay();
	MMOARPGCharacterBase = Cast<AMMOARPGCharacterBase>(GetOwner());
	if (MMOARPGCharacterBase.IsValid()) {
		// 初始化ASC.
		AbilitySystemComponent = Cast<UMMOARPGAbilitySystemComponent>(MMOARPGCharacterBase->GetAbilitySystemComponent());

		const FName InKey = TEXT("NormalAttack");
		/* 仅运行在服务器的逻辑. */
		if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority) {
			// 往Skill池子里写入 从DTRow里查出来的名叫"NormalAttack"的普攻连招.
			AddMMOARPGGameplayAbility_ToSkillpool(InKey, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK);
			// 往Skills整个池子里写入注册 闪避
			AddMMOARPGGameplayAbility_ToSkillpool(TEXT("Dodge"), EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
			// 往Skills整个池子里写入注册 冲刺
			AddMMOARPGGameplayAbility_ToSkillpool(TEXT("Sprint"), EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);

			// 仅允许服务器注册ASC的持有对象(即人物基类.).
			AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
		}
		// 在连招触发器实例的内部, 使用GA:平砍 写入它; ROLE_SimulatedProxy模拟玩家也需要写入连招触发器.
		this->RegisterComboAttack(ComboAttackCheck, InKey);
	}
}

// 添加并授权某技能. 返回技能实例的句柄.
FGameplayAbilitySpecHandle UFightComponent::AddAbility(TSubclassOf<UGameplayAbility> InNewAbility)
{
	if (IsValid(InNewAbility) && AbilitySystemComponent.IsValid()) {
		return AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(InNewAbility));
	}

	return FGameplayAbilitySpecHandle();
}

// 从技能池里找指定名字的GA.
UMMOARPGGameplayAbility* UFightComponent::GetGameplayAbility(const FName& InKey)
{
	if (FGameplayAbilitySpecHandle* InHandle = Skills.Find(InKey)) {
		if (AbilitySystemComponent.IsValid()) {
			if (FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(*InHandle)) {
				return Cast<UMMOARPGGameplayAbility>(Spec->Ability);
			}
		}
	}
	return nullptr;
}

// 按指定名字, 在Skills大池子里查找技能并激活.
void UFightComponent::Attack_TriggerGA(const FName& InKey)
{
	if (AbilitySystemComponent.IsValid()) {// 检查弱指针ASC是否被破坏.
		if (InKey != TEXT("")) {
			if (FGameplayAbilitySpecHandle* Handle = Skills.Find(InKey)) {
				AbilitySystemComponent->TryActivateAbility(*Handle);
			}
		}
	}
}

// 放闪避技能.
void UFightComponent::DodgeSkill_Implementation()
{
	Attack_TriggerGA(TEXT("Dodge"));
}

// 放冲刺技能. 广播至其他客户端
void UFightComponent::SprintSkill_Implementation()
{
	Attack_TriggerGA(TEXT("Sprint"));
}

// 往Skills池子里写入 从DTRow里查出来的指定GA的形式攻击.
void UFightComponent::AddMMOARPGGameplayAbility_ToSkillpool(const FName& InKey_GAName, EMMOARPGGameplayAbilityType GAType /*= EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK*/)
{
	if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {
		// 用GameState找出人身上配的 DTRow::技能表
		if (FCharacterSkillTable* InSkillTable_row = InGS->GetCharacterSkillTable(MMOARPGCharacterBase->GetID())) {
			// 从DTR里拿表中的TMAP作为数据源.
			auto GetMMOAPRGGameplayAbility = [&](EMMOARPGGameplayAbilityType InGAType) ->TMap<FName, TSubclassOf<UGameplayAbility>>* {
				switch (InGAType) {
					case GAMEPLAYABILITY_SKILLATTACK:
					{
						return &(InSkillTable_row->SkillAttack);
						break;
					}
					case GAMEPLAYABILITY_COMBOATTACK:
					{
						return &(InSkillTable_row->ComboAttack);
						break;
					}
				}
				return nullptr;
			};
			// DT单行里查找缓存池,并按名字找到GA,	往Skills池子里写入这个GA
			if (GetMMOAPRGGameplayAbility(GAType) != nullptr) {
				if (TSubclassOf<UGameplayAbility>* InGameplayAbility = GetMMOAPRGGameplayAbility(GAType)->Find(InKey_GAName)) {
					Skills.Add(InKey_GAName, AddAbility(*InGameplayAbility));
				}
			}
		}
	}
}

// 往Skill池子里写入 从DTRow里查出来的指定名字的Skill形式攻击.
void UFightComponent::AddSkillAttack(const FName& InKey)
{
	if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {// 再拿GS
		if (FCharacterSkillTable* InSkillTable_row = InGS->GetCharacterSkillTable(MMOARPGCharacterBase->GetID())) {
			// DT单行里查找Skill缓存池,并按名字找到GA,	往Skills池子里写入这个GA
			if (TSubclassOf<UGameplayAbility>* InGameplayAbility = InSkillTable_row->SkillAttack.Find(InKey)) {
				Skills.Add(InKey, AddAbility(*InGameplayAbility));
			}
		}
	}
}

// 往Skill池子里写入 从DTRow里查出来的指定名字的普攻连招.
void UFightComponent::AddComboAttack(const FName& InKey)
{
	if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {// 再拿GS
		if (FCharacterSkillTable* InSkillTable_row = InGS->GetCharacterSkillTable(MMOARPGCharacterBase->GetID())) {
			// DT单行里查找连击缓存池,并按名字找到GA,	往Skill池子里写入这个GA
			if (TSubclassOf<UGameplayAbility>* InGameplayAbility = InSkillTable_row->ComboAttack.Find(InKey)) {
				Skills.Add(InKey, AddAbility(*InGameplayAbility));
			}
		}
	}
}

/** 用指定GA去注册连招触发器黑盒. */
void UFightComponent::RegisterComboAttack(FSimpleComboCheck& InComboAttackCheck, const FName& InGAName)
{
	InComboAttackCheck.Character_CombatInterface = MMOARPGCharacterBase.Get();
	InComboAttackCheck.ComboKey_GA = InGAName;
	if (UMMOARPGGameplayAbility* GameplayAbility = GetGameplayAbility(InGAName)) {/*先按名字从技能池里找GA,并把触发器的段数注册成GA里蒙太奇段数.*/
		InComboAttackCheck.MaxIndex = GameplayAbility->GetCompositeSectionsNumber();
	}
	else {/*没找到GA就给个4段. */
		InComboAttackCheck.MaxIndex = 4;
	}
}

// 广播触发器Press至其他客户端; 由服务器广播到其他的客户端.
void UFightComponent::Press_Implementation()
{
	ComboAttackCheck.Press();
}

// 广播触发器Release至其他客户端; 由服务器广播到其他的客户端.
void UFightComponent::Released_Implementation()
{
	ComboAttackCheck.Released();
}

// 广播触发器Rest至其他客户端; 由服务器广播到其他的客户端.
void UFightComponent::Reset_Implementation()
{
	ComboAttackCheck.Reset();
}
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

	// 初始化 持有本组件的人物基类.
	MMOARPGCharacterBase = Cast<AMMOARPGCharacterBase>(GetOwner());
	
	if (MMOARPGCharacterBase.IsValid()) {
		// 初始化 人里的ASC.	
		AbilitySystemComponent = Cast<UMMOARPGAbilitySystemComponent>(MMOARPGCharacterBase->GetAbilitySystemComponent());

		// 仅允许服务器注册ASC的持有对象(即人物基类.).
		if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority) {
			AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
		}
	}
// 	MMOARPGCharacterBase = Cast<AMMOARPGCharacterBase>(GetOwner());
// 	if (MMOARPGCharacterBase.IsValid()) {
// 		// 初始化ASC.
// 		AbilitySystemComponent = Cast<UMMOARPGAbilitySystemComponent>(MMOARPGCharacterBase->GetAbilitySystemComponent());
// 
// 		const FName InKey = TEXT("Player.Attack.ComboLinkage");
// 		/* 仅运行在服务器的逻辑. */
// 		if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority) {
// 			// 往Skill池子里写入 从DTRow里查出来的名叫"NormalAttack"的普攻连招.
// 			AddMMOARPGGameplayAbility_ToSkillpool(InKey, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK);
// 			// 往Skills整个池子里写入注册 闪避
// 			AddMMOARPGGameplayAbility_ToSkillpool(TEXT("Player.Skill.Dodge"), EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
// 			// 往Skills整个池子里写入注册 冲刺
// 			AddMMOARPGGameplayAbility_ToSkillpool(TEXT("Player.Skill.Sprint"), EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
// 			// 往Skills整个池子里写入注册 冲刺2
// 			AddMMOARPGGameplayAbility_ToSkillpool(TEXT("Player.Skill.Sprint2"), EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
// 			// 往池子里写入 受击能力
// 			AddMMOARPGGameplayAbility_ToSkillpool(TEXT("Player.State.Hit"), EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_NONE);
//  			// 往池子里写入 死亡能力
//  			AddMMOARPGGameplayAbility_ToSkillpool(TEXT("Player.State.Die"), EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_NONE);
// 
// 			// 仅允许服务器注册ASC的持有对象(即人物基类.).
// 			AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
// 		}
// 		// 在连招触发器实例的内部, 使用GA:平砍 写入它; ROLE_SimulatedProxy模拟玩家也需要写入连招触发器.
// 		this->RegisterComboAttack(ComboAttackCheck, InKey);
// 	}
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
	Attack_TriggerGA(TEXT("Player.Skill.Dodge"));
}

// 放冲刺技能. 广播至其他客户端
void UFightComponent::SprintSkill_Implementation()
{
	Attack_TriggerGA(TEXT("Player.Skill.Sprint"));
}

void UFightComponent::Sprint2Skill_Implementation()
{
	Attack_TriggerGA(TEXT("Player.Skill.Sprint2"));
}

// 放受击技能
void UFightComponent::Hit()
{
	if (AbilitySystemComponent.IsValid()) {
		AbilitySystemComponent->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Player.State.Hit"))));
	}
}

// 放死亡 技能
void UFightComponent::Die()
{
	if (AbilitySystemComponent.IsValid()) {
		AbilitySystemComponent->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Player.State.Die"))));
	}
}

/** 往Skills大池子里注册写入  指定Tag的技能. */
void UFightComponent::AddMMOARPGGameplayAbility_ToSkillpool(const FName& InKey_GAName, EMMOARPGGameplayAbilityType GAType /*= EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK*/)
{
	if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {
		// 用GameState找出人身上配的 DTRow::技能表
		if (FCharacterSkillTable* InSkillTable_row = InGS->GetCharacterSkillTable(MMOARPGCharacterBase->GetID())) {
			// 从DTR里拿表中的TMAP作为数据源.
			auto GetMMOAPRGGameplayAbility = [&](EMMOARPGGameplayAbilityType InGAType) ->TSubclassOf<UGameplayAbility>* {
				switch (InGAType) {
					case GAMEPLAYABILITY_LIMBS:
					{
						return InSkillTable_row->FindLimbs(InKey_GAName);
						break;
					}
					case GAMEPLAYABILITY_SKILLATTACK:
					{
						return InSkillTable_row->FindSkillAttack(InKey_GAName);
						break;
					}
					case GAMEPLAYABILITY_COMBOATTACK:
					{
						return InSkillTable_row->FindComboAttack(InKey_GAName);
						break;
					}
				}
				return nullptr;
			};

			// DT单行里查找缓存池,并按名字找到GA,	往Skills池子里写入这个GA
			if (GetMMOAPRGGameplayAbility(GAType) != nullptr) {
				if (TSubclassOf<UGameplayAbility>* InGameplayAbility = GetMMOAPRGGameplayAbility(GAType)) {

					/* 按技能形式来源切分, 分肢体形式的 和 真正技能形式的*/
					if (GAType != EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_LIMBS) {
						Skills.Add(InKey_GAName, AddAbility(*InGameplayAbility));// 给大池子注册一队pair
					}
					else {/* 肢体行为的能力(如受击, 死亡, 嘲讽)*/
						AddAbility(*InGameplayAbility);// 直接give技能, 不需要往大池子里注册.
					}
				}
			}
			//
		}
	}
}

// 往Skill池子里写入 从DTRow里查出来的指定名字的Skill形式攻击.
void UFightComponent::AddSkillAttack(const FName& InKey)
{
	if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {// 再拿GS
		if (FCharacterSkillTable* InSkillTable_row = InGS->GetCharacterSkillTable(MMOARPGCharacterBase->GetID())) {
			// DT单行里查找Skill缓存池,并按名字找到GA,	往Skills池子里写入这个GA
			if (TSubclassOf<UGameplayAbility>* InGameplayAbility = InSkillTable_row->FindComboAttack(InKey)) {
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
			if (TSubclassOf<UGameplayAbility>* InGameplayAbility = InSkillTable_row->FindSkillAttack(InKey)) {
				Skills.Add(InKey, AddAbility(*InGameplayAbility));
			}
		}
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

// 注册各部分技能(按形式来源)
void UFightComponent::RegisterGameplayAbility(const TArray<FName>& InGANames, EMMOARPGGameplayAbilityType InGASrcEnum)
{
	// 核验 基类人和ASC
	if (MMOARPGCharacterBase.IsValid() && AbilitySystemComponent.IsValid()) {
// 		 const FName InKey = TEXT("Player.Attack.ComboLinkage");

		/* 仅运行在服务器的逻辑. */
		if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority) {
			// 往Skill池子里写入 从DTRow里查出来的一组	GA
			for (auto& Tmp : InGANames) {
				AddMMOARPGGameplayAbility_ToSkillpool(Tmp, InGASrcEnum); 
			}

			// 仅在combo来源形式下 广播 "用一组GA注册连招黑盒"
			if (InGASrcEnum == EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK) {
				RegisterComboAttackMulticast(InGANames);
			}
		}
// 		if (InGASrcEnum == EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK) {
// 			// 在连招触发器实例的内部, 使用GA:平砍 写入它; ROLE_SimulatedProxy模拟玩家也需要写入连招触发器.
// 			this->RegisterComboAttack(ComboAttackCheck, InKey);
// 		}
	}
}

/** 用1个GA去注册1个连招黑盒. */
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

// 用一组GA去注册1个连招黑盒
void UFightComponent::RegisterComboAttack(const TArray<FName>& InGANames)
{
	for (auto& Tmp : InGANames) {
		RegisterComboAttack(ComboAttackCheck, Tmp);
	}
}

// 广播 "用一组GA注册连招黑盒"
void UFightComponent::RegisterComboAttackMulticast_Implementation(const TArray<FName>& InGANames)
{
	RegisterComboAttack(InGANames);
}

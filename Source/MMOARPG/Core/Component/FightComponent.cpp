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

		// 往Skill池子里写入 从DTRow里查出来的名叫"NormalAttack"的普攻连招.
		AddComboAttack(TEXT("NormalAttack"));
		// 注册ASC的持有对象(即人物基类.).
		AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
	}
	// 在连招触发器实例的内部, 使用GA:平砍 写入它.
	this->RegisterComboAttack(ComboAttackCheck, TEXT("NormalAttack"));
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
				Cast<UMMOARPGGameplayAbility>(Spec->Ability);
			}
		}
	}
	return nullptr;
}

// 放GA: 普攻.
void UFightComponent::NormalAttack(const FName& InKey)
{
	if (AbilitySystemComponent.IsValid()) {// 检查弱指针ASC是否被破坏.
		// 查技能池里的平砍技能并触发它.
		if (FGameplayAbilitySpecHandle* Handle = Skills.Find(TEXT("NormalAttack"))) {
			AbilitySystemComponent->TryActivateAbility(*Handle);
		}
	}
}

// 往Skill池子里写入 从DTRow里查出来的指定名字的Skill形式攻击.
void UFightComponent::AddSkillAttack(const FName& InKey)
{

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

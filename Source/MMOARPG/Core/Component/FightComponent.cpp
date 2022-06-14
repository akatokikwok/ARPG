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
		AbilitySystemComponent = Cast<UMMOARPGAbilitySystemComponent>(MMOARPGCharacterBase->GetAbilitySystemComponent());// 初始化ASC.

		if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {// 再拿GS
			// 技能池里添加1个名叫"NormalAttack"的平砍技能. 数据源是来自GS的DataTable.
			if (FCharacterSkillTable* InSkillTable = InGS->GetCharacterSkillTable(MMOARPGCharacterBase->GetID())) {
				Skills.Add(TEXT("NormalAttack"), AddAbility(InSkillTable->NormalAttack));
			}
		}
		// 注册ASC的持有对象(即人物基类.).
		AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
	}
	// 在连招触发器实例的内部,为普攻这个技能注册数据.
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

// 拿到技能池里指定名字的技能实例.
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

// 注册连击触发器内部数据.
void UFightComponent::RegisterComboAttack(FSimpleComboCheck& InComboAttackCheck, const FName& InKey)
{
	InComboAttackCheck.Character = MMOARPGCharacterBase.Get();
	InComboAttackCheck.ComboKey = InKey;

	if (UMMOARPGGameplayAbility* GameplayAbility = GetGameplayAbility(InKey)) {
		InComboAttackCheck.MaxIndex = GameplayAbility->GetCompositeSectionsNumber();
	}
	else {/*没找到就给个默认4.f. */
		InComboAttackCheck.MaxIndex = 4;
	}
}

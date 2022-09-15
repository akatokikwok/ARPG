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
	// 			// 仅允许服务器注册ASC的持有对象(即人物基类.)..
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

// 从连招池子里提1个GA并激活.
bool UFightComponent::Attack_TriggerGA(const FName& InGAKey)
{
	return TryActivateAbility(InGAKey, ComboAttacks);
}

// 从某种GA缓存池里提出给定名字的GA并激活它, 可能会激活失败
bool UFightComponent::TryActivateAbility(const FName& InTagName, const TMap<FName, FGameplayAbilitySpecHandle>& InMap)
{
	// 检查弱指针ASC是否被破坏.
	if (AbilitySystemComponent.IsValid()) {
		if (const FGameplayAbilitySpecHandle* Handle = InMap.Find(InTagName)) {
			AbilitySystemComponent->TryActivateAbility(*Handle);
			return true;
		}
	}
	return false;
}

// 放闪避技能.
void UFightComponent::DodgeSkill/*_Implementation*/()
{
	if (AbilitySystemComponent.IsValid()) {
		TryActivateAbility(TEXT("Player.Skill.Dodge"), Skills);// 从Skills缓存池里激活名为"Player.Skill.Dodge" 的闪避GA
	}
}

// 放冲刺技能. 广播至其他客户端
void UFightComponent::SprintSkill/*_Implementation*/()
{
	if (AbilitySystemComponent.IsValid()) {
		TryActivateAbility(TEXT("Player.Skill.Sprint"), Skills);// 从Skills缓存池里激活名为"Player.Skill.Sprint" 的冲刺GA
	}
}

// 激活 受击技能
void UFightComponent::Hit()
{
	if (AbilitySystemComponent.IsValid()) {
		static const FName Player_State_Hit_Name(TEXT("Player.State.Hit"));
		TryActivateAbility(Player_State_Hit_Name, Limbs);// 从肢体缓存池里激活名为Player.State.Hit的受击GA
	}
}

// 激活 死亡技能
void UFightComponent::Die()
{
	if (AbilitySystemComponent.IsValid()) {
		static const FName Character_State_Die_Name(TEXT("Player.State.Die"));
		TryActivateAbility(Character_State_Die_Name, Limbs);// 从肢体缓存池里激活名为Player.State.Die的死亡GA
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
					/* 按技能形式来源切分, 分三类*/
					switch (GAType) {
						case GAMEPLAYABILITY_SKILLATTACK:
							Skills.Add(InKey_GAName, AddAbility(*InGameplayAbility));// 为skill池子添加元素
							break;
						case GAMEPLAYABILITY_COMBOATTACK:
							ComboAttacks.Add(InKey_GAName, AddAbility(*InGameplayAbility));// 为连招池子添加元素
							break;
						case GAMEPLAYABILITY_LIMBS:
							Limbs.Add(InKey_GAName, AddAbility(*InGameplayAbility));// 为肢体池子添加元素
							break;
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
void UFightComponent::Press()
{
	ComboAttackCheck.Press();
}

// 广播触发器Release至其他客户端; 由服务器广播到其他的客户端.
void UFightComponent::Released()
{
	ComboAttackCheck.Released();
}

// 广播触发器Rest至其他客户端; 由服务器广播到其他的客户端.
void UFightComponent::Reset()
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

			// 			// 仅在combo来源形式下 广播 "用一组GA注册连招黑盒"
			// 			if (InGASrcEnum == EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK) {
			// 				RegisterComboAttackMulticast(InGANames);
			// 			}
		}
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
// void UFightComponent::RegisterComboAttackMulticast(const TArray<FName>& InGANames)
// {
// 	RegisterComboAttack(InGANames);
// }

// 当血量变化时候处理
void UFightComponent::HandleHealth(AMMOARPGCharacterBase* InstigatorPawn, AActor* DamageCauser, const struct FGameplayTagContainer& InTags, float InNewValue)
{
	if (MMOARPGCharacterBase.IsValid()) {
		// 目标任务是否死亡(死亡后会执行奖励buff结算)
		if (MMOARPGCharacterBase->IsDie()) {
			/* 对施法者 进行击杀战利结算贡献.种类是 "击杀奖励buff".*/
			InstigatorPawn->RewardEffect(MMOARPGCharacterBase->GetCharacterLevel(),
				MMOARPGCharacterBase->GetDeathRewardEffect(),
				[&]() ->void {
					// 施法者是否满足了升等级条件. 满足则给一份新的递归式的buff奖励, 种类是 "施法者提升经验值buff".
					this->UpdateLevel(InstigatorPawn);
				}
			);

			/* 对目标人物 执行使其死亡 最后执行.*/
			MMOARPGCharacterBase->PlayDie();
		}
		else {
			// 施法者和受击序号必须有意义.
			if (InstigatorPawn != nullptr) {
				if (MMOARPGCharacterBase->GetHitID() != INDEX_NONE) {
					// 让挨打的人面朝向 施法攻击者.
					FRotator TargetRot = (-InstigatorPawn->GetActorForwardVector()).ToOrientationRotator();
					MMOARPGCharacterBase->SetActorRotation(TargetRot);

					// 对目标人物, 再执行让它挨打受击.
					MMOARPGCharacterBase->PlayHit();
				}
			}
		}
	}
}

// 当蓝量变化时候处理
void UFightComponent::HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue)
{

}

void UFightComponent::RewardEffect(float InNewLevel, TSubclassOf<UGameplayEffect> InNewRewardBuff, TFunction<void()> InFun_AppendLogic)
{
	if (AbilitySystemComponent.IsValid()) {
		// 升等级.
		UpdateLevel(InNewLevel, InNewRewardBuff);

		// 执行传进来的附加逻辑段.
		InFun_AppendLogic();
	}
}

// 指明一个Pawn, 为其升等级.
void UFightComponent::UpdateLevel(AMMOARPGCharacterBase* InUpgradeLevelPawn)
{
	if (InUpgradeLevelPawn->IsUpdateLevel()) {
		InUpgradeLevelPawn->RewardEffect(InUpgradeLevelPawn->GetCharacterLevel() + 1, // 升一级
			InUpgradeLevelPawn->GetUpgradeRewardEffect(), // 提升经验条buff
			[&]() ->void { UpdateLevel(InUpgradeLevelPawn); } // 嵌套递归式升级.
		);
	}
}

// 升自己等级.
void UFightComponent::UpdateLevel(float InLevel, TSubclassOf<UGameplayEffect> InNewReward)
{
	if (AbilitySystemComponent.IsValid()) {
		// 断言警告提示, 若奖励GE未配置则会提醒开发者.
		checkf(InNewReward, TEXT("这张奖励机制的GE 必须在蓝图中被配置."));

		// 给自身应用 奖励机制GE
		AbilitySystemComponent->ApplyGameplayEffectToSelf(Cast<UGameplayEffect>(InNewReward->GetDefaultObject()),
			InLevel,
			AbilitySystemComponent->MakeEffectContext()
		);
	}
}

// 从技能缓存池里提出所有技能名字
void UFightComponent::GetSkillTagsName(TArray<FName>& OutNames)
{
	Skills.GetKeys(OutNames);
}

// 从连招缓存池里提出所有连招名字
void UFightComponent::GetComboAttackTagsName(TArray<FName>& OutNames)
{
	ComboAttacks.GetKeys(OutNames);
}

// 从肢体缓存池里提出所有肢体动作名字
void UFightComponent::GetLimbsTagsName(TArray<FName>& OutNames)
{
	Limbs.GetKeys(OutNames);
}

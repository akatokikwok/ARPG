﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../Game/Abilities/MMOARPGAbilitySystemComponent.h"
#include "SimpleComboType.h"
#include "../Game/Abilities/MMOARPGGameplayAbility.h"
#include "../../MMOARPGGameType.h"
#include "FightComponent.generated.h"


/**
 * 战斗组件.继承自MotionComp
 */
UCLASS()
class MMOARPG_API UFightComponent : public UMotionComponent
{
	GENERATED_BODY()
public:
	UFightComponent();
	virtual void BeginPlay() override;

	// 从技能池里找指定名字的GA.
	UMMOARPGGameplayAbility* GetGameplayAbility(const FName& InKey);

	// 从连招池子里提1个GA并激活.
	UFUNCTION(BlueprintCallable)
		bool Attack_TriggerGA(const FName& InKey);// 放GA: 普攻.

	// 从某种GA缓存池里提出给定名字的GA并激活它, 可能会激活失败
	bool TryActivateAbility(const FName& InTagName, const TMap<FName, FGameplayAbilitySpecHandle>& InMap);

	// 往Skill池子里写入 从DTRow里查出来的指定名字的形式攻击.
	void AddMMOARPGGameplayAbility_ToSkillpool(const FName& InKey_GAName, EMMOARPGGameplayAbilityType GAType = EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);

	// 往Skill池子里写入 从DTRow里查出来的指定名字的Skill形式攻击.
	void AddSkillAttack(const FName& InKey);

	// 往Skill池子里写入 从DTRow里查出来的指定名字的普攻连招.
	void AddComboAttack(const FName& InKey);
protected:
	/** 用指定GA去注册连招触发器黑盒. */
	void RegisterComboAttack(FSimpleComboCheck& InComboAttackCheck, const FName& InGAName);

	// 添加并授权某技能. 返回技能实例的句柄.
	FGameplayAbilitySpecHandle AddAbility(TSubclassOf<UGameplayAbility> InNewAbility);
public:
	// 拿连击触发器.
	FSimpleComboCheck* GetSimpleComboInfo() { return &ComboAttackCheck; }

	// 广播触发器Press至其他客户端; 由服务器广播到其他的客户端.
	/*UFUNCTION(NetMulticast, Reliable)*/
	void Press();
	// 广播触发器Release至其他客户端; 由服务器广播到其他的客户端.
	/*UFUNCTION(NetMulticast, Reliable)*/
	void Released();
	// 广播触发器Rest至其他客户端; 由服务器广播到其他的客户端.
	/*UFUNCTION(NetMulticast, Reliable)*/
	void Reset();

	// 放闪避技能. 广播至其他客户端
	//UFUNCTION(NetMulticast, Reliable)
	void DodgeSkill();// 放闪避技能; 广播至其他客户端

// 放冲刺技能. 广播至其他客户端
//UFUNCTION(NetMulticast, Reliable)
	void SprintSkill();// 放冲刺技能; 广播至其他客户端

// 放冲刺2技能. 广播至其他客户端
//UFUNCTION(NetMulticast, Reliable)
	//void Sprint2Skill();// 放冲刺2技能; 广播至其他客户端

	// 激活 受击技能
	UFUNCTION(BlueprintCallable)
		void Hit();

	// 激活 死亡技能
	UFUNCTION(BlueprintCallable)
		void Die();

public:
	// 注册各部分技能(按形式来源)
	void RegisterGameplayAbility(const TArray<FName>& InGANames/*一组技能名*/, EMMOARPGGameplayAbilityType InGASrcEnum/*技能形式来源*/);

	// "单机版" 用一组GA去注册1个连招黑盒
	void RegisterComboAttack(const TArray<FName>& InGANames);

	// 广播 "用一组GA注册连招黑盒"
	/*UFUNCTION(NetMulticast, Reliable)*/
// 		void RegisterComboAttackMulticast(const TArray<FName>& InGANames);

public:
	// 当血量变化时候处理
	virtual void HandleHealth(AMMOARPGCharacterBase* InstigatorPawn, AActor* DamageCauser, const struct FGameplayTagContainer& InTags, float InNewValue);

	// 当蓝量变化时候处理
	virtual void HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue);

	// 击杀授予的奖励buff结算.
	virtual void RewardEffect(float InNewLevel, TSubclassOf<UGameplayEffect> InNewReward, TFunction<void()> InFun_AppendLogic);

	// 指明一个Pawn, 为其升等级.
	void UpdateLevel(AMMOARPGCharacterBase* InUpgradeLevelPawn);

	// 升自己等级.
	void UpdateLevel(float InLevel, TSubclassOf<UGameplayEffect> InNewReward);

public:
	// 从技能缓存池里提出所有技能名字
	void GetSkillTagsName(TArray<FName>& OutNames);

	// 从连招缓存池里提出所有连招名字
	void GetComboAttackTagsName(TArray<FName>& OutNames);

	// 从肢体缓存池里提出所有肢体动作名字
	void GetLimbsTagsName(TArray<FName>& OutNames);

private:
	/**来自人物基类的ASC
	 * 战斗组件也持有1个ASC
	 * 弱指针,需要检查是否被销毁.
	 */
	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TWeakObjectPtr<UMMOARPGAbilitySystemComponent> AbilitySystemComponent;

	// Combo形式的攻击 触发器
	UPROPERTY()
		FSimpleComboCheck ComboAttackCheck;

public:
	// 受击ID
	UPROPERTY()
		int32 HitID;// 受击ID

protected:
	// 技能(如冲刺,躲闪)缓存池,  1个名字对应1个GA句柄
	TMap<FName, FGameplayAbilitySpecHandle> Skills;
	// 连招缓存池
	TMap<FName, FGameplayAbilitySpecHandle> ComboAttacks;
	// 肢体行为缓存池
	TMap<FName, FGameplayAbilitySpecHandle> Limbs;
};

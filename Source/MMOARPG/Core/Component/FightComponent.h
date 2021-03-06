// Fill out your copyright notice in the Description page of Project Settings.

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

	// 按指定名字, 在Skills大池子里查找技能并激活.
	UFUNCTION(BlueprintCallable)
		void Attack_TriggerGA(const FName& InKey);// 放GA: 普攻.

	
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
	UFUNCTION(NetMulticast, Reliable)
		void Press();
	// 广播触发器Release至其他客户端; 由服务器广播到其他的客户端.
	UFUNCTION(NetMulticast, Reliable)
		void Released();
	// 广播触发器Rest至其他客户端; 由服务器广播到其他的客户端.
	UFUNCTION(NetMulticast, Reliable)
		void Reset();

	// 放闪避技能. 广播至其他客户端
	UFUNCTION(NetMulticast, Reliable)
		void DodgeSkill();// 放闪避技能; 广播至其他客户端

	// 放冲刺技能. 广播至其他客户端
	UFUNCTION(NetMulticast, Reliable)
		void SprintSkill();// 放冲刺技能; 广播至其他客户端

// 	// 放冲刺2技能. 广播至其他客户端
// 	UFUNCTION(NetMulticast, Reliable)
// 		void Sprint2Skill();// 放冲刺2技能; 广播至其他客户端

	// 放受击 技能
	UFUNCTION( BlueprintCallable)
		void Hit();

	// 放死亡 技能
	UFUNCTION(BlueprintCallable)
		void Die();

public:
	// 注册各部分技能(按形式来源)
	void RegisterGameplayAbility(const TArray<FName>& InGANames/*一组技能名*/, EMMOARPGGameplayAbilityType InGASrcEnum/*技能形式来源*/);

	// "单机版" 用一组GA去注册1个连招黑盒
	void RegisterComboAttack(const TArray<FName>& InGANames);

	// 广播 "用一组GA注册连招黑盒"
	UFUNCTION(NetMulticast, Reliable)
		void RegisterComboAttackMulticast(const TArray<FName>& InGANames);

		/// //////////////////////////////////////////////////////////////////////////
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
protected:
	// GA缓存池, 1个技能名对1个技能句柄
	TMap<FName, FGameplayAbilitySpecHandle> Skills;

public:
	// 受击ID
	UPROPERTY()
		int32 HitID;// 受击ID

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../Game/Abilities/MMOARPGAbilitySystemComponent.h"
#include "SimpleComboType.h"
#include "../Game/Abilities/MMOARPGGameplayAbility.h"
#include "FightComponent.generated.h"

// 攻击形式来源枚举.
enum EMMOARPGGameplayAbilityType
{
	GAMEPLAYABILITY_SKILLATTACK,// 从属技能形式的攻击.
	GAMEPLAYABILITY_COMBOATTACK,// 从属combo形式的攻击.
};

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

	// 放冲刺2技能. 广播至其他客户端
	UFUNCTION(NetMulticast, Reliable)
		void Sprint2Skill();// 放冲刺2技能; 广播至其他客户端

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
	// 能力或者技能缓存池.
	TMap<FName, FGameplayAbilitySpecHandle> Skills;

private:
	FName KeyNameUsedIter = FName();
};

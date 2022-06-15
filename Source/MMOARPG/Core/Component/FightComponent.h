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

	// 放GA: 普攻.
	UFUNCTION(BlueprintCallable)
		void NormalAttack(const FName& InKey);// 放GA: 普攻.

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

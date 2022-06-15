﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../Game/Abilities/MMOARPGAbilitySystemComponent.h"
#include "SimpleComboType.h"
#include "../Game/Abilities/MMOARPGGameplayAbility.h"
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

	// 添加并授权某技能. 返回技能实例的句柄.
	FGameplayAbilitySpecHandle AddAbility(TSubclassOf<UGameplayAbility> InNewAbility);

	// 从技能池里找指定名字的GA.
	UMMOARPGGameplayAbility* GetGameplayAbility(const FName& InKey);

	// 放GA: 普攻.
	UFUNCTION(BlueprintCallable)
		void NormalAttack(const FName& InKey);
protected:
	/** 用指定GA去注册连招触发器黑盒. */
	void RegisterComboAttack(FSimpleComboCheck& InComboAttackCheck, const FName& InGAName);

public:
	FSimpleComboCheck* GetSimpleComboInfo() { return &ComboAttackCheck; }// 拿连击触发器.

	/// //////////////////////////////////////////////////////////////////////////
private:
	/**来自人物基类的ASC
	 * 战斗组件也持有1个ASC
	 * 弱指针,需要检查是否被销毁.
	 */
	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TWeakObjectPtr<UMMOARPGAbilitySystemComponent> AbilitySystemComponent;

	// Combo触发器
	UPROPERTY()
	FSimpleComboCheck ComboAttackCheck;
protected:
	// 能力或者技能缓存池.
	TMap<FName, FGameplayAbilitySpecHandle> Skills;

};

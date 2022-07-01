// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../SimpleComboType.h"
#include "SimpleCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class USimpleComboInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 让人继承的I接口,负责技能连击Combo逻辑的部分实现.
 */
class SIMPLECOMBAT_API ISimpleComboInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 执行信号处事件.
	UFUNCTION(BlueprintCallable, Category = "Combat")
		virtual void AnimSignal(int32 InSignal) {};

	// 连招攻击(需指明具体属于哪个技能).
	UFUNCTION(BlueprintCallable, Category = "Combat")
		virtual void ComboAttack(const FName& InKey) {}

	// 获取连招检测器.
	virtual struct FSimpleComboCheck* GetSimpleComboInfo() { return nullptr; };

	// 读取敌对目标人物或者怪物
	UFUNCTION(BlueprintCallable, Category = "Combat")
		virtual AActor* GetTarget() { return NULL; }
};

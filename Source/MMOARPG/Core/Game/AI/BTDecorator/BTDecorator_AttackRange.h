﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTDecorator_AttackRange.generated.h"

/**
 * 攻击范围黑板.
 */
UCLASS()
class MMOARPG_API UBTDecorator_AttackRange : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
public:/// 覆写一些黑板接口.

	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const;

	/** Notify called after GameplayTask finishes initialization (not active yet) */
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}
};

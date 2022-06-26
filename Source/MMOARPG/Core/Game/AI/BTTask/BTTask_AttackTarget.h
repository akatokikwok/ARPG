#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackTarget.generated.h"

/**
 * BTTask: 攻击敌人.派生自虚幻的 UBTTaskNode.
 */
UCLASS()
class MMOARPG_API UBTTask_AttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

/// 覆写一些派生自虚幻的接口
public:
	/** Notify called after GameplayTask finishes initialization (not active yet) */
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override {}

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}

public:
	// 覆写BTTaskNode::ExecuteTask; 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	// 覆写BTNode::InitializeFromAsset.
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

public:
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Blackboard_Actor;// 黑板.
};
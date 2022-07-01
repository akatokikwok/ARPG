#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MMOARPGBTServiceBase.generated.h"

/**
 * MMOARPG项目里 BTService. 派生自虚幻的UBTService.
 */
UCLASS()
class MMOARPG_API UMMOARPGBTServiceBase : public UBTService
{
	GENERATED_BODY()
/// 覆写BTNode的一些接口.
public:

	/** Notify called after GameplayTask finishes initialization (not active yet) */
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override {}

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}
//
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

/// 黑板资产的各个field.
public:
	// 敌人.
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
		struct FBlackboardKeySelector BlackBoardKey_Target;
	// 敌人与自己的距离.
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
		struct FBlackboardKeySelector BlackBoardKey_Distance;
	// 敌人位置.
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
		struct FBlackboardKeySelector BlackBoardKey_Location;
	// 自身死亡情况.
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
		struct FBlackboardKeySelector BlackBoardKey_Death;
};
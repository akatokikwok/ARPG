#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Cooldown.h"
#include "BTDecorator_SimpleCooldown.generated.h"

/**
 * 冷却的黑板; 派生自虚幻的UBTDecorator_Cooldown.
 */
UCLASS()
class MMOARPG_API UBTDecorator_SimpleCooldown : public UBTDecorator_Cooldown
{
	GENERATED_BODY()

public:/// 覆写一些接口.

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	/** Notify called after GameplayTask finishes initialization (not active yet) */
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}
};
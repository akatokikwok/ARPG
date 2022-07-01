#pragma once

#include "CoreMinimal.h"
#include "Core/MMOARPGBTServiceBase.h"
#include "MMOARPGBossBTService.generated.h"

/**
 * BOSS用的BTService.
 */
UCLASS()
class MMOARPG_API UMMOARPGBossBTService : public UMMOARPGBTServiceBase
{
	GENERATED_BODY()
public:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};

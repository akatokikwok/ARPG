#include "BTTask_AttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "../AIController/Core/MMOARPGAIControllerBase.h"

// 覆写BTTaskNode::ExecuteTask; 
EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Failed;
}

// 覆写BTNode::InitializeFromAsset.
void UBTTask_AttackTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 拿到黑板资产并解析进字段.
	if (UBlackboardData* BBAsset = GetBlackboardAsset()) {
		Blackboard_Actor.ResolveSelectedKey(*BBAsset);
	}
}

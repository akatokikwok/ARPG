#include "BTTask_AttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "../AIController/Core/MMOARPGAIControllerBase.h"

// 覆写BTTaskNode::ExecuteTask; 
EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent()) {
		if (AMMOARPGAIControllerBase* AIConrollerBase = Cast<AMMOARPGAIControllerBase>(OwnerComp.GetOwner())) {// 拿到AIC
			if (AMMOARPGCharacterBase* InTarget = Cast<AMMOARPGCharacterBase>(MyBlackBoard->GetValueAsObject(Blackboard_Actor.SelectedKeyName))) {

				AIConrollerBase->Attack(InTarget);// 让AIC去控制自身执行攻击
				return EBTNodeResult::Succeeded;
			}
		}
	}
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

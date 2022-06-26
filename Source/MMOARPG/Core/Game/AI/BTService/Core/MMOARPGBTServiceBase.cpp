#include "MMOARPGBTServiceBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../AIController/Core/MMOARPGAIControllerBase.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"

//
void UMMOARPGBTServiceBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

}

//
void UMMOARPGBTServiceBase::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 解析黑板资产至各field.
	if (UBlackboardData* BBAsset = GetBlackboardAsset()) {
		BlackBoardKey_Target.ResolveSelectedKey(*BBAsset);
		BlackBoardKey_Distance.ResolveSelectedKey(*BBAsset);
		BlackBoardKey_Location.ResolveSelectedKey(*BBAsset);
		BlackBoardKey_Death.ResolveSelectedKey(*BBAsset);
	}
}

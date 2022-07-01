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

	//
	if (BlackBoardKey_Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() &&
		BlackBoardKey_Distance.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() &&
		BlackBoardKey_Location.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() &&
		BlackBoardKey_Death.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass()) 
	{
		if (AMMOARPGAIControllerBase* OwnerController = Cast<AMMOARPGAIControllerBase>(OwnerComp.GetOwner())) {
			if (AMMOARPGCharacterBase* OwnerCharacter = Cast<AMMOARPGCharacterBase>(OwnerController->GetPawn())) {
				if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent()) {
					MyBlackBoard->SetValueAsBool(BlackBoardKey_Death.SelectedKeyName, OwnerCharacter->IsDie());// 人的死亡情况赋值到黑板 DeathKey里.
					
					if (OwnerCharacter->IsDie()) {// AIC控制的自身已经死亡
						MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, 9999999.f);
						MyBlackBoard->SetValueAsVector(BlackBoardKey_Location.SelectedKeyName, OwnerCharacter->GetActorLocation());
						return;
					}

					/* 自身未死亡的情况. */
					/* 尝试获取一次黑板里的target字段能否转成人形态的object; */ 
					AMMOARPGCharacterBase* InTarget = Cast<AMMOARPGCharacterBase>(MyBlackBoard->GetValueAsObject(BlackBoardKey_Target.SelectedKeyName));
					if (InTarget) {
						// 距离判断,合理就索敌, 超出就返回刷怪出生点.
						float Distance = FVector::Dist(InTarget->GetActorLocation(), OwnerCharacter->GetActorLocation());
						if (Distance <= 1280.f) {
							MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, Distance);// 设定黑板里距离为 "敌人到AIC控制的自身 的距离"
							MyBlackBoard->SetValueAsVector(BlackBoardKey_Location.SelectedKeyName, InTarget->GetActorLocation());// 设定黑板里敌人位置为 "目标敌人位置"
						}
						else {
							MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, nullptr);
						}
						
					}
					/* 黑板里不存在这样的charbase. */
					else {
						
						// 再让AIC搜寻一次敌人.
						InTarget = OwnerController->FindTarget();
						if (InTarget != nullptr) {
							float Distance = FVector::Dist(InTarget->GetActorLocation(), OwnerCharacter->GetActorLocation());
							MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, Distance);// 让黑板键去记录 "敌人到自身的距离".
						}
						else {
							MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, 9999999.f);
							MyBlackBoard->SetValueAsVector(BlackBoardKey_Location.SelectedKeyName, OwnerCharacter->GetActorLocation());
						}

						MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, InTarget);
					}

				}
			}
		}
	}
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

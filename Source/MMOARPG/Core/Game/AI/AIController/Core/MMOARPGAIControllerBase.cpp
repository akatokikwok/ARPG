﻿#include "MMOARPGAIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "../../../../../MMOARPGGameMethod.h"
#include "ThreadManage.h"

// Beginplay.
void AMMOARPGAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

// 设定黑板组件里的敌人.
void AMMOARPGAIControllerBase::SetTargetForce(AMMOARPGCharacterBase* InTarget)
{
	if (GetBlackboardComponent()) {
		GetBlackboardComponent()->SetValueAsObject("Target", InTarget);
	}
}

// 获取黑板组件里的敌人
AMMOARPGCharacterBase* AMMOARPGAIControllerBase::GetTarget()
{
	if (GetBlackboardComponent()) {
		return Cast<AMMOARPGCharacterBase>(GetBlackboardComponent()->GetValueAsObject("Target"));
	}

	return NULL;
}

// AIC控制自己去 搜寻敌人.
AMMOARPGCharacterBase* AMMOARPGAIControllerBase::FindTarget()
{
	if (AMMOARPGCharacterBase* InPawn = Cast<AMMOARPGCharacterBase>(GetPawn())) {
		TArray<ECharacterType> IgnoreTypes;
		// 不把怪物型和BOSS型识别为敌人,阻止同类怪打同类怪
		if (InPawn->GetCharacterType() == ECharacterType::CHARACTER_MONSTER || InPawn->GetCharacterType() == ECharacterType::CHARACTER_BOSS) {
			IgnoreTypes.Add(ECharacterType::CHARACTER_MONSTER);
			IgnoreTypes.Add(ECharacterType::CHARACTER_BOSS);
		}
		return MMOARPGGameMethod::FindTarget(Cast<AMMOARPGCharacterBase>(GetPawn()), IgnoreTypes, 1024);
	}

	return nullptr;
}

// 让自身执行攻击
void AMMOARPGAIControllerBase::Attack(AMMOARPGCharacterBase* InTarget)
{
	if (AMMOARPGCharacterBase* OwnerCharacter = Cast<AMMOARPGCharacterBase>(GetPawn())) {
		OwnerCharacter->NormalAttack(TEXT("Player.Attack.ComboLinkage"));
	}
}

// 让自身执行攻击(按标签)
void AMMOARPGAIControllerBase::Attack(const FName& InTag)
{
	if (AMMOARPGCharacterBase* OwnerCharacter = Cast<AMMOARPGCharacterBase>(GetPawn())) {
		OwnerCharacter->NormalAttack(InTag);
	}
}

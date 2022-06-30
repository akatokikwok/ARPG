#include "MMOARPGGameMethod.h"
#include "Core/Game/Character/Core/MMOARPGCharacterBase.h"
#include "EngineUtils.h"

namespace MMOARPGGameMethod
{
	/**
	 * 查找半径内最近的敌对目标.
	 */
	AMMOARPGCharacterBase* FindTarget(AMMOARPGCharacterBase* InThis, float InRange)
	{
		AMMOARPGCharacterBase* Target = NULL;
		if (InThis && !InThis->IsDie() && InThis->GetWorld()) {
			for (TActorIterator<AMMOARPGCharacterBase> It(InThis->GetWorld(), AMMOARPGCharacterBase::StaticClass()); It; ++It) {
				if (AMMOARPGCharacterBase* NewTarget = *It) {
					if (!NewTarget->IsDie() && InThis != NewTarget) {// 敌对目标不能是自己而且敌对目标必须存活

						float Distance = FVector::Dist(NewTarget->GetActorLocation(), InThis->GetActorLocation());
						if (Distance <= InRange) {
							InRange = Distance;
							Target = NewTarget;
						}
					}
				}
			}
		}

		return Target;
	}

	/** 敌我识别用, 求取人物身份类型 */
	ECharacterType GetCharacterType(int32 InCharacterID)
	{
		if (InCharacterID == 1) {
			return ECharacterType::CHARACTER_PLAYER_MAIN;
		}
		else if (InCharacterID > 1 && InCharacterID <= 4096) {
			return ECharacterType::CHARACTER_PARTNER;
		}
		else if (InCharacterID > 4097 && InCharacterID <= 8119) {
			return ECharacterType::CHARACTER_NPC_RESIDENT;
		}
		else if (InCharacterID > 8119 && InCharacterID <= 16423) {
			return ECharacterType::CHARACTER_NPC_GUARDIAN;
		}
		else if (InCharacterID > 16423 && InCharacterID <= 30000) {
			return ECharacterType::CHARACTER_MONSTER;
		}
		else if (InCharacterID > 30000 && InCharacterID < 100000) {
			return ECharacterType::CHARACTER_BOSS;
		}

		return ECharacterType::CHARACTER_NONE;
	}

}
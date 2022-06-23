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
}
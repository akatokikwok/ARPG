#pragma once

#include "CoreMinimal.h"
#include "ComboSkillHitCollision.generated.h"

/**
 * 生成的碰撞actor,用于普攻攻击时候检测.
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API AHitCollision : public AActor
{
	GENERATED_BODY()
public:
	AHitCollision();

};
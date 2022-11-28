#pragma once

#include "CoreMinimal.h"

/**
 * 管理数值计算的一个算法数据结构
 */
namespace MMOARPGNumericalCalculation
{
	float GetDamage(
		float ActivePhysicsAttack,
		float ActiveMagicAttack,
		float PassiveMagicDefense,
		float PassivePhysicsDefense,
		float DamageBase);
}
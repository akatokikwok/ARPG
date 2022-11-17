#pragma once

#include "CoreMinimal.h"

namespace MMOARPGNumericalCalculation
{
	float GetDamage(
		float ActivePhysicsAttack,
		float ActiveMagicAttack,
		float PassiveMagicDefense,
		float PassivePhysicsDefense,
		float DamageBase);
}
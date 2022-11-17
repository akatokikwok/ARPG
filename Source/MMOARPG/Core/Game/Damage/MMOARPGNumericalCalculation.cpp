#include "MMOARPGNumericalCalculation.h"

namespace MMOARPGNumericalCalculation
{
	float GetDamage(
		float ActivePhysicsAttack,
		float ActiveMagicAttack,
		float PassiveMagicDefense,
		float PassivePhysicsDefense,
		float DamageBase)
	{
		float PhysicsAttackDamage = ActivePhysicsAttack * (100 / (100 + PassivePhysicsDefense));
		float MagicAttackDamage = ActiveMagicAttack * (100 / (100 + PassiveMagicDefense));

		return PhysicsAttackDamage + MagicAttackDamage + DamageBase;
	}
}
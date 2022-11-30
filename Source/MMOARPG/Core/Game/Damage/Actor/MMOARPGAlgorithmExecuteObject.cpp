#include "MMOARPGAlgorithmExecuteObject.h"
 #include "../MMOARPGNumericalCalculation.h"

UMMOARPGAlgorithmExecuteObject::UMMOARPGAlgorithmExecuteObject()
	:Super()
{

}

float UMMOARPGAlgorithmExecuteObject::GetFloatAlgorithmValue(const FNAEParam& InParam)
{
	//不需要执行蓝图
//	Super::GetFloatAlgorithmValue(InParam);
	if (InParam.Key == TEXT("PhysicsAttack")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("MagicAttack")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("MagicDefense")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("PhysicsDefense")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("MaxEmpiricalValue")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("Health")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("Mana")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("DamageBase")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("ExperienceReward")) {
		return InParam.Count * InParam.Coefficient + InParam.Value;
	}
	else if (InParam.Key == TEXT("AttackRange")) {
		return InParam.Value;
	}

	return 0.0f;
}

int32 UMMOARPGAlgorithmExecuteObject::GetInt32AlgorithmValue(const FNAEParam& InParam)
{
	//不需要执行蓝图
//	Super::GetInt32AlgorithmValue(InParam);

	if (InParam.Key == TEXT("Level")) {
		return InParam.Count;//因为是等级直接返回
	}
	else if (InParam.Key == TEXT("ID")) {
		return InParam.Value;//因为是等级直接返回
	}

	return 0;
}

float UMMOARPGAlgorithmExecuteObject::GetDamageAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData)
{
	//不需要执行蓝图
//	Super::GetDamageAlgorithmValue(InLvActiveData,InLvPassiveData);
	float ActivePhysicsAttack = 0.f;
	float ActiveMagicAttack = 0.f;
	float PassiveMagicDefense = 0.f;
	float PassivePhysicsDefense = 0.f;
	float DamageBase = 0.f;

	if (const float* InActivePhysicsAttack = InLvActiveData.Find(TEXT("PhysicsAttack"))) {
		if (const float* InActiveMagicAttack = InLvActiveData.Find(TEXT("MagicAttack"))) {
			if (const float* InPassiveMagicDefense = InLvPassiveData.Find(TEXT("MagicDefense"))) {
				if (const float* InPassivePhysicsDefense = InLvPassiveData.Find(TEXT("PhysicsDefense"))) {

					if (const float* InDamageBase = InLvPassiveData.Find(TEXT("DamageBase"))) {
						ActivePhysicsAttack = *InActivePhysicsAttack;
						ActiveMagicAttack = *InActiveMagicAttack;
						PassiveMagicDefense = *InPassiveMagicDefense;
						PassivePhysicsDefense = *InPassivePhysicsDefense;

						DamageBase = *InDamageBase;
					}
					
				}
				else {
					return 0.0f;
				}
			}
			else {
				return 0.0f;
			}
		}
		else {
			return 0.0f;
		}
	}
	else {
		return 0.0f;
	}

	return MMOARPGNumericalCalculation::GetDamage(
		ActivePhysicsAttack,
		ActiveMagicAttack,
		PassiveMagicDefense,
		PassivePhysicsDefense,
		DamageBase);
}

float UMMOARPGAlgorithmExecuteObject::GetTreatmentAlgorithmValue(const TMap<FName, float>& InLvActiveData, const TMap<FName, float>& InLvPassiveData)
{
	return 0.f;
}
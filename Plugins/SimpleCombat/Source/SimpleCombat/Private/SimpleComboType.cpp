#include "../Public/SimpleComboType.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

FContinuousReleaseSpell::FContinuousReleaseSpell()
	: ContinuousReleaseSpellIndex(INDEX_NONE)
	, BuffPtr(NULL)
{

}

void FContinuousReleaseSpell::Press()
{
	if (ContinuousReleaseSpellIndex == INDEX_NONE) {
		ContinuousReleaseSpellIndex++;
	}
}

void FContinuousReleaseSpell::Released()
{
	// 设定为2号即认为是释放
	ContinuousReleaseSpellIndex = 2;
}

void FContinuousReleaseSpell::Reset()
{
	// 持续施法复位
	ContinuousReleaseSpellIndex = INDEX_NONE;
}

FSimpleComboCheck::FSimpleComboCheck()
	: ComboIndex(INDEX_NONE)
	, bLongPress(false)
	, bShortPress(false)
	, Character_CombatInterface(nullptr)
	, MaxIndex(0)
{

}

/** 在总段数内循环递增刷新段号. */
void FSimpleComboCheck::UpdateComboIndex()
{
	check(MaxIndex > 0);
	ComboIndex++;
	if (ComboIndex > MaxIndex) {// 溢出则重置段号.
		ComboIndex = 1;
	}
}

/** Press()激发连击触发器黑盒 */
void FSimpleComboCheck::Press()
{
	if (ComboIndex == INDEX_NONE) {
		ComboIndex++;
		check(Character_CombatInterface);
		if (Character_CombatInterface != nullptr) {
			Character_CombatInterface->ComboAttack(ComboKey_GA);
		}
	}
	bShortPress = true;
	bLongPress = true;
}

// 主动结束连击触发器黑盒.
void FSimpleComboCheck::Released()
{
	bLongPress = false;// 松开时候认为 仅有长按被认为解除.
}

// 释放并重置总的状态.
void FSimpleComboCheck::Reset()
{
	ComboIndex = INDEX_NONE;// 还原连招段号.
}

namespace SimpleComboType
{
	FVector GetCurrentCharacterDirection(ACharacter* InCharacter, const FVector& InDirectionForce)
	{
		FVector V = FVector::ZeroVector;
		if (InCharacter) {
			if (InDirectionForce.X != 0.f) {
				V += InCharacter->GetActorForwardVector() * InDirectionForce.X;
			}

			if (InDirectionForce.Y != 0.f) {
				V += InCharacter->GetActorRightVector() * InDirectionForce.Y;
			}

			if (InDirectionForce.Z != 0.f) {
				V += InCharacter->GetActorUpVector() * InDirectionForce.Z;
			}
		}

		return V;
	}
}
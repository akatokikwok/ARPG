#include "../Public/SimpleComboType.h"
#include "CombatInterface/SimpleCombatInterface.h"

FSimpleComboCheck::FSimpleComboCheck()
	: ComboIndex(INDEX_NONE)
	, bLongPress(false)
	, bShortPress(false)
	, Character(nullptr)
	, MaxIndex(0)
{

}

void FSimpleComboCheck::UpdateComboIndex()
{
	check(MaxIndex > 0);
	ComboIndex++;
	if (ComboIndex > MaxIndex) {// 溢出则重置段号.
		ComboIndex = 1; 
	}
}

void FSimpleComboCheck::Press()
{
	if (ComboIndex == INDEX_NONE) {
		ComboIndex++;
		Character->ComboAttack(ComboKey);
	}
	bShortPress = true;
	bLongPress = true;
}

void FSimpleComboCheck::Released()
{
	bLongPress = false;// 松开时候认为 仅有长按被认为解除.
}

void FSimpleComboCheck::Reset()
{
	ComboIndex = INDEX_NONE;// 还原连招段号.
}


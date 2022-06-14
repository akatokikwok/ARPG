#include "../Public/SimpleComboType.h"
#include "CombatInterface/SimpleCombatInterface.h"

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
		Character_CombatInterface->ComboAttack(ComboKey);
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


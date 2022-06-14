#include "../Public/SimpleComboType.h"

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

}

void FSimpleComboCheck::Press()
{

}

void FSimpleComboCheck::Released()
{

}

void FSimpleComboCheck::Reset()
{
	ComboIndex = INDEX_NONE;// 还原连招段号.
}


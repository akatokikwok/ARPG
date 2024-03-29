﻿#include "UI_CharacterAppearance.h"
#include "Components/WrapBox.h"
#include "Components/ComboBoxString.h"

void UUI_CharacterAppearance::NativeConstruct()
{
	Super::NativeConstruct();
	AppearanceType->OnSelectionChanged.AddDynamic(this, &UUI_CharacterAppearance::SelectAppearanceType);
}

void UUI_CharacterAppearance::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterAppearance::SelectAppearanceType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

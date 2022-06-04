// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_PartnerInfo.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UI_PartnerInfo.h"

void UUI_PartnerInfo::NativeConstruct()
{
	Super::NativeConstruct();

	ClickedCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_PartnerInfo::OnClickedCharacter);
}

void UUI_PartnerInfo::NativeDestruct()
{
	Super::NativeDestruct();


}

void UUI_PartnerInfo::OnClickedCharacter(bool bClicked)
{

}

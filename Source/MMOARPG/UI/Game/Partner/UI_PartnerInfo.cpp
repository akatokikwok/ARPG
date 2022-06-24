// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_PartnerInfo.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UI_PartnerList.h"
#include "../../../Core/Game/MMOARPGPlayerController.h"

UUI_PartnerInfo::UUI_PartnerInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CharacterID(INDEX_NONE)
{

}

void UUI_PartnerInfo::NativeConstruct()
{
	Super::NativeConstruct();

	ClickedCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_PartnerInfo::OnClickedCharacter);
}

void UUI_PartnerInfo::NativeDestruct()
{
	Super::NativeDestruct();


}

void UUI_PartnerInfo::ShowSelected(bool bShow)
{
	ClickedCheckBox->SetCheckedState(bShow == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UUI_PartnerInfo::SetCharacterName(const FText& NewName)
{
	CharacterName->SetText(NewName);
}

void UUI_PartnerInfo::OnClickedCharacter(bool bClicked)
{
	/** 把敲击的人物框设定为选中状态. */
	if (UUI_PartnerList* InList = GetParents<UUI_PartnerList>()) {// 拿到垂直框.
		InList->CallCharacterListByPredicate([&](UUI_PartnerInfo* InInfo) ->bool {
			if (InInfo->GetCharacterID() == CharacterID) {
				InInfo->ShowSelected(true);// 只有ID号匹配才设为选定状态
			}
			else {
				InInfo->ShowSelected(false);
			}
			
			return true;// 仿函数要求这里继续遍历.
		});
	}

	// 使用控制器替换人物.
	if (AMMOARPGPlayerController* InPlayerController = GetPlayerController<AMMOARPGPlayerController>()) {
		InPlayerController->ReplaceCharacter(CharacterID);
	}

}

// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_PartnerList.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI_PartnerInfo.h"

void UUI_PartnerList::NativeConstruct()
{
	Super::NativeConstruct();
	Init();// 初始化外观.
}

void UUI_PartnerList::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_PartnerList::Init()
{
	if (UI_PartnerInfoClass != nullptr) {
		for (int32 i = 1; i < 5; i++) {
			if (UUI_PartnerInfo* InPartner = CreateWidget<UUI_PartnerInfo>(GetWorld(), UI_PartnerInfoClass)) {
				if (i == 1) {// 如果是1号主人物.就默认设置为选中状态.
					InPartner->ShowSelected(true);
				}

				InPartner->SetParents(this);

				// 把子元素控件添加至垂直框.
				if (UVerticalBoxSlot* InBoxSlot = Cast<UVerticalBoxSlot>(CharacterList->AddChildToVerticalBox(InPartner))) {
					InPartner->SetCharacterID(i);// 每构造一次就设定一下子元素的ID.
					InBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));// 设定一下间距.
				}
			}
		}
	}
}

void UUI_PartnerList::CallCharacterListByPredicate(TFunction<bool(UUI_PartnerInfo* Info)> InFun)
{
	for (int32 i = 0; i < CharacterList->GetChildrenCount(); i++) {
		if (UUI_PartnerInfo* InInfo = Cast<UUI_PartnerInfo>(CharacterList->GetChildAt(i))) {
			// 反复遍历子控件, 直至满足 仿函数操作该子控件失败.
			if (!InFun(InInfo)) {
				break;
			}
		}
	}
}

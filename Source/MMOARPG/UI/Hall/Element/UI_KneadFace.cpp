// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_KneadFace.h"

void UUI_KneadFace::NativeConstruct()
{
	Super::NativeConstruct();

 	Body->OnCheckStateChanged.AddDynamic(this, &UUI_KneadFace::OnClickedBody);
 	Appearance->OnCheckStateChanged.AddDynamic(this, &UUI_KneadFace::OnClickedAppearance);
 	Talent->OnCheckStateChanged.AddDynamic(this, &UUI_KneadFace::OnClickedTalent);
 
 	Body->SetCheckedState(ECheckBoxState::Checked);// 默认 UCheckBox-Body是敲击状态.
}

void UUI_KneadFace::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_KneadFace::OnClickedBody(bool bClicked)
{
	Body->SetCheckedState(ECheckBoxState::Checked);
	Appearance->SetCheckedState(ECheckBoxState::Unchecked);
	Talent->SetCheckedState(ECheckBoxState::Unchecked);

	FacePanel->SetActiveWidgetIndex(0);// 手动设定切换器渲染0号控件,也就是Modeling(蓝图中赋值).
}

void UUI_KneadFace::OnClickedAppearance(bool bClicked)
{
	Body->SetCheckedState(ECheckBoxState::Unchecked);
	Appearance->SetCheckedState(ECheckBoxState::Checked);
	Talent->SetCheckedState(ECheckBoxState::Unchecked);

	FacePanel->SetActiveWidgetIndex(1);// 手动设定切换器渲染1号控件,也就是Appearance(蓝图中赋值).
}

void UUI_KneadFace::OnClickedTalent(bool bClicked)
{
	Body->SetCheckedState(ECheckBoxState::Unchecked);
	Appearance->SetCheckedState(ECheckBoxState::Unchecked);
	Talent->SetCheckedState(ECheckBoxState::Checked);

	FacePanel->SetActiveWidgetIndex(2);// 手动设定切换器渲染2号控件,也就是Talent(蓝图中赋值).
}

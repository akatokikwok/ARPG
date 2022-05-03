// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Print.h"

void UUI_Print::PlayTextAnim()
{
	UUI_Base::PlayWidgetAnim(TEXT("PlayText"));// PlayText在蓝图里设定好,他是一个UMG动画名称.
}

void UUI_Print::SetText(const FText& InText)
{
	MsgLog->SetText(InText);
}

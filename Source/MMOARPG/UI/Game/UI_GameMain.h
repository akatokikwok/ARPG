// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_MainBase.h"
#include "../../MMOARPGMacroType.h"
#include "../../Robot/MMOARPGRobot.h"
#include "UI_GameMain.generated.h"

class UUI_Print;
/**
 * 类似于LoginMain, HallMain
 * 切换到Game,之后会激活这个类.
 */
UCLASS()
class MMOARPG_API UUI_GameMain : public UUI_MainBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg) override;
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

protected:
	FMMOARPGRobot Robot;// 仅测试用机器人.
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_MainBase.h"
#include "../../MMOARPGMacroType.h"

#if UE_MMOARPG_DEBUG_DS
#include "../../Robot/MMOARPGRobot.h"
#endif// UE_MMOARPG_DEBUG_DS

#include "Character/UI_CharacterHealthState.h"
#include "UI_GameMain.generated.h"
class UUI_Print;
class UUI_CharacterHealthState;
class UProgressBar;
class UUI_SkillPage;
class UUI_UnderSkillGroup;

/**
 * 类似于LoginMain, HallMain
 * 切换到Game,之后会激活这个类.
 */
UCLASS()
class MMOARPG_API UUI_GameMain : public UUI_MainBase
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UUI_CharacterHealthState* MainCharacterHealthState;
	
	// 窗口技能页
	UPROPERTY(meta = (BindWidget))
		UUI_SkillPage* SkillPage;

	// 横排技能组
	UPROPERTY(meta = (BindWidget))
		UUI_UnderSkillGroup* UnderSkillGroup;

	// 经验进度条,
	UPROPERTY(meta = (BindWidget))
		UProgressBar* EXPBar;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg) override;
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

protected:
#if UE_MMOARPG_DEBUG_DS
	FMMOARPGRobot Robot;// 仅测试用机器人.
#endif
};

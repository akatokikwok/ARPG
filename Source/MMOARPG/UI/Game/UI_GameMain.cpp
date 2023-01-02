// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_GameMain.h"
#include "ThreadManage.h"
#include "Character/UI_CharacterHealthState.h"
#include "../../Core/Game/Abilities/MMOARPGAttributeSet.h"
#include "Components/ProgressBar.h"
#include "../../Core/Game/Character/Core/MMOARPGCharacterBase.h"

#if UE_MMOARPG_DEBUG_DS
#include "../../Core/Game/Character/MMOARPGPlayerCharacter.h"
#endif

#define LOCTEXT_NAMESPACE "UI_GameMain"

void UUI_GameMain::NativeConstruct()
{
	Super::NativeConstruct();

#if UE_MMOARPG_DEBUG_DS// 若开启调试.
	if (AMMOARPGPlayerCharacter* InPlayer = GetPawn<AMMOARPGPlayerCharacter>()) {
		Robot.GetGateRobot().StartDelegate.BindUObject(InPlayer, &AMMOARPGPlayerCharacter::FlushKneadingRequest);// 网关机器人的代理绑定1个人物刷新人物样貌的回调.
	}

	/// 让指定的用户号存档切换
	/**
	 * 在数据库机器上的wp_usermeta表上查看创建出来的2个角色的用户号user_id字段
	 */
	if (GameCount == 0) {
		Robot.InitUser(17, 0);// 手动写死测试, 8号用户0号存档
		GameCount++;
	}
	else if (GameCount == 1) {
		Robot.InitUser(18, 0);// 手动写死测试, 9号用户0号存档
		GameCount++;
	}

	Robot.InitGate("127.0.0.1", 11222);// 11222是MMOARPGGateServer的ini里配置.
#endif

// 	// 延迟0.5秒 模拟机器人.
// 	GThread::Get()->GetCoroutines().BindLambda(0.5f, [=]() {
// 		Robot.RunRobot();		
// 	});
}

void UUI_GameMain::NativeDestruct()
{
	Super::NativeDestruct();
#if UE_MMOARPG_DEBUG_DS
	GameCount = 0;
#endif
}

void UUI_GameMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	/* 机器人Tick. */
#if UE_MMOARPG_DEBUG_DS
	Robot.Tick(InDeltaTime);
#endif// UE_MMOARPG_DEBUG_DS


	// 客户端看到UI应用属性集变化的效果
	if (MainCharacterHealthState && GetWorld() && GetWorld()->GetFirstPlayerController()) {
		if (AMMOARPGCharacterBase* InPawn = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacterBase>()) {// 确保玩家是受controller管理的.
			if (UMMOARPGAttributeSet* InAttribute = InPawn->GetAttribute()) {
				// 血条 蓝条 UI上显示.
				MainCharacterHealthState->SetHealth(InAttribute->GetHealth() / InAttribute->GetMaxHealth());
				MainCharacterHealthState->SetHealthValue(InAttribute->GetMaxHealth(), InAttribute->GetHealth());
				MainCharacterHealthState->SetMana(InAttribute->GetMana() / InAttribute->GetMaxMana());
				MainCharacterHealthState->SetManaValue(InAttribute->GetMaxMana(), InAttribute->GetMana());

				// 经验值显示 
				EXPBar->SetPercent(InAttribute->GetEmpiricalValue() / InAttribute->GetMaxEmpiricalValue());

				// 实时等级显示
				MainCharacterHealthState->SetLv(InAttribute->GetLevel());
			}
		}
	}
}

void UUI_GameMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS) {

	}
}

void UUI_GameMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber) {
		case 101:
		{
			break;
		}
	}
}
#undef LOCTEXT_NAMESPACE


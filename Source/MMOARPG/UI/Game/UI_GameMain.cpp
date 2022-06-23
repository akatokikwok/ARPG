// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_GameMain.h"
#include "ThreadManage.h"
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

	// 让指定的用户号存档切换
	if (GameCount == 0) {
		Robot.InitUser(1, 3);// 手动写死测试, 1号用户3号存档
		GameCount++;
	}
	else if (GameCount == 1) {
		Robot.InitUser(3, 3);// 手动写死测试, 3号用户3号存档
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

	GameCount = 0;
}

void UUI_GameMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
#if UE_MMOARPG_DEBUG_DS
	Robot.Tick(InDeltaTime);
#endif// UE_MMOARPG_DEBUG_DS

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


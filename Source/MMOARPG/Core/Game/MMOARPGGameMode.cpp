// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGGameMode.h"
#include "Character/MMOARPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "../../Core/Game/MMOARPGHUD.h"
#include "../Common/MMOARPGGameInstance.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "MMOARPGPlayerState.h"
#include "MMOARPGGameState.h"
#include "Character/MMOARPGPlayerCharacter.h"

AMMOARPGGameMode::AMMOARPGGameMode()
{
	HUDClass = AMMOARPGHUD::StaticClass();// HUD暂设为 Game-HUD,游玩时候的HUD.
	PlayerStateClass = AMMOARPGPlayerState::StaticClass();// 注册PS
	GameStateClass = AMMOARPGGameState::StaticClass();// 注册GS

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMMOARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	/**
	 * 只在服务端执行.
	 * 链接至指定端口号的服务器(可以是Center/DS服务器)并 循环绑定创建客户端接收响应协议的回调RecvProtocol.
	 */
	LinkServer();
}

void AMMOARPGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController()) {
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(mRecvDelegate);
		}
	}
}

void AMMOARPGGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** 玩家登录到DS后会激活的1个接口. */
void AMMOARPGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	// 使用协程,让这一步有时间去准备.延迟0.5s是因为DS准备好之后,要给予客户端一些时间执行同步.
	GThread::Get()->GetCoroutines().BindLambda(
		0.5f, [&](APlayerController* InNewController) ->void {
			if (InNewController != nullptr) {
				// to do.
				
				if (AMMOARPGPlayerCharacter* InPawn = InNewController->GetPawn<AMMOARPGPlayerCharacter>()) {// 先拿人
					if (AMMOARPGGameState* InGS = GetGameState<AMMOARPGGameState>()) {// 再拿GS
						// 把GS里的动画数据解算到Player身上.
						if (FCharacterAnimTable* InAnimRowData = InGS->GetCharacterAnimTable(InPawn->GetID())) {
							InPawn->AnimTable = InAnimRowData;
						}
					}
				}
			}
		}, NewPlayer
	);
}

void AMMOARPGGameMode::BindClientRcv()
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController()) {
			mRecvDelegate = InGameInstance->GetClient()->GetController()->RecvDelegate.AddLambda(
				[&](uint32 ProtocolNumber, FSimpleChannel* Channel) {
					this->RecvProtocol(ProtocolNumber, Channel);
				});
		}
		else {
			GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
		}
	}
	else {
		GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
	}
}

void AMMOARPGGameMode::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{

}

void AMMOARPGGameMode::LinkServer()
{
	//创建客户端
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient()) {
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &AMMOARPGGameMode::LinkServerInfo);

			//InGameInstance->LinkServer();
			//
			// 作为测试,11231是中心服务器端口.
			// 即暂用 链接到本机的中心服务器.
			InGameInstance->LinkServer(TEXT("127.0.0.1"), 11231);

			BindClientRcv();
		}
	}
}

void AMMOARPGGameMode::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{

}

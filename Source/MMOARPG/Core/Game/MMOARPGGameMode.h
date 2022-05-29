// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGGameMode.generated.h"
class FSimpleChannel;


/**
 * 主要使用的GM.
 */
UCLASS(minimalapi)
class AMMOARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMMOARPGGameMode();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

private:
	//
	void BindClientRcv();
	//
	UFUNCTION()
		void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);
	// 链接至指定端口号的服务器并 循环绑定创建客户端接收响应协议的回调RecvProtocol.
	void LinkServer();
	//
	void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

private:
	FDelegateHandle mRecvDelegate;
};




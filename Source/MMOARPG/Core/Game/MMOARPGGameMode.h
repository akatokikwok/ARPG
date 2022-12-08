// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.h"
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
	/** 玩家登录到DS后会激活的1个接口. */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/** 让登录到DS的角色发送捏身材请求. */
	void LoginCharacterUpdateKneadingRequest(int32 InUserID);

	/** 向CS服务器发送 gas人物属性集请求 */
	void GetCharacterDataRequests(int32 InUserID, int32 InCharacterID, int32 MMOARPGSlot);

	// 更新人物属性请求.
	void UpdateAttributeRequests(int32 InUserID, int32 InCharacterID, MMOARPGCharacterAttributeType InAttributeType, float InValue);

	// 升级人物等级请求.
	void UpdateLevelRequests(int32 InUserID, int32 InCharacterID, const FMMOARPGCharacterAttribute& InCharacterAttribute);

	// 经过DS,向CS发送重生请求
	void CharacterResurrectionRequests(int32 InUserID, int32 InCharacterID);

protected:
	//	向CS发送身份覆写请求.
	void IdentityReplicationRequests();
private:
	//
	void BindClientRcv();
	//
	UFUNCTION()
		void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);
	// 链接至指定端口号的服务器并 循环绑定创建客户端接收响应协议的回调RecvProtocol.
	void LinkServer();

private:
	/// 当DS接收到来自中心服务器的回复.
	void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

private:
	FDelegateHandle mRecvDelegate;
};




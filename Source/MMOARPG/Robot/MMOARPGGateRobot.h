#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"

class FMMOARPGGateRobot
{
	friend class FMMOARPGRobot;
public:
	FMMOARPGGateRobot();
	virtual ~FMMOARPGGateRobot();

	void Init(const FString& Host, const int32 Port);
	virtual void Tick(float DeltaTime);
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

	// 发这个协议是为了向网关注册玩家id,
	void Run_CharacterAppearanceRequests();
	// 发这个协议是为了 登录,在登录到的DS服务器注册1个玩家.
	void Run_LoginToDSServerRequests();
public:
	FSimpleDelegate StartDelegate;// 代理: 当登录到DS后. (在UI构造时候绑定 人身上RPC的刷新相貌请求.)
private:
	FSimpleNetManage* GateClient;// 模拟网关的客户端.
	int32 UserID;// 用户号.
	int32 SlotID;// 人物槽位号.
};
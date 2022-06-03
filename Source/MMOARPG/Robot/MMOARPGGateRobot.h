#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"

class FMMOARPGGateRobot
{
public:
	FMMOARPGGateRobot();
	virtual ~FMMOARPGGateRobot();

	void Init(const FString& Host, const int32 Port);
	virtual void Tick(float DeltaTime);
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

private:
	FSimpleNetManage* GateClient;// 模拟网关的客户端.
};
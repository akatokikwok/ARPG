#include "MMOARPGGateServerObject.h"
#include "SimpleMySQLibrary.h"
#include "Log\MMOARPGGateServerLog.h"
#include "Protocol/ServerProtocol.h"
#include "MMOARPGType.h"

void UMMOARPGGateServerObejct::Init()
{
	Super::Init();
	
}

void UMMOARPGGateServerObejct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void UMMOARPGGateServerObejct::Close()
{
	Super::Close();

}

void UMMOARPGGateServerObejct::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_GateStatusRequests:
		{
			FMMOARPGGateStatus Status;
			GetAddrInfo(Status.GateServerAddrInfo);// 先获取地址.
			Status.GateConnectionNum = GetManage()->GetConnetionNum();// 再获取链接数量.

			SIMPLE_PROTOCOLS_SEND(SP_GateStatusResponses, Status);// 本服务器往外发一个 响应协议.

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("SP_GateStatusRequests"));

			break;
		}
	}
}
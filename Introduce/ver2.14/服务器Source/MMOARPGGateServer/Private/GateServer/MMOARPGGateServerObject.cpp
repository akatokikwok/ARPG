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
			// 只能获取到当前服务器链接到的那个对象的地址(网关的地址).
			GetLocalAddrInfo(Status.GateServerAddrInfo);// 使用GetLocalAddrInfo这个API 直接获取网关的地址.
			
			Status.GateConnectionNum = GetManage()->GetConnetionNum();// 再获取链接数量.

			SIMPLE_PROTOCOLS_SEND(SP_GateStatusResponses, Status);// 本服务器往外发一个 响应协议.

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("SP_GateStatusRequests"));// 网关服务器往外发送请求.

			break;
		}
	}
}
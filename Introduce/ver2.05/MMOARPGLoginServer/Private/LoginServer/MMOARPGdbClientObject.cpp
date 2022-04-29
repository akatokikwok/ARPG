#include "MMOARPGdbClientObject.h"
#include "Log/MMOARPGLoginServerLog.h"
#include "../../MMOARPGCommon/Source/MMOARPGCommon/Public/Protocol/LoginProtocol.h"

void UMMOARPGdbClientObject::Init()
{
	Super::Init();

}

void UMMOARPGdbClientObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void UMMOARPGdbClientObject::Close()
{
	Super::Close();

}

void UMMOARPGdbClientObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	// db服务器准备好数据之后,它会执行回调.
	// db会通知当前的Login服务器, 说白了就是通知Login服务器里的dbClientObject.
	switch (InProtocol) {
		case SP_LoginResponses : // 响应协议.
		{
			
			break;
		}
	
	}

}

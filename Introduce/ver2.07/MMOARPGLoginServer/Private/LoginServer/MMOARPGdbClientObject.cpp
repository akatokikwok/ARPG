#include "MMOARPGdbClientObject.h"
#include "Log/MMOARPGLoginServerLog.h"
#include "../../MMOARPGCommon/Source/MMOARPGCommon/Public/Protocol/LoginProtocol.h"
#include "MMOARPGType.h"
#include "SimpleProtocolsDefinition.h"
#include "ServerList.h"

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
		/// 这个dbClientObject实际上就是响应dbServer发过来的协议.
		case SP_LoginResponses : // 响应dbServer协议.
		{
			FString StringTmp;
			FSimpleAddrInfo AddrInfo;
			ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;

			// dbClinet这里扮演的角色,是负责 先收取从dbServer发过来的协议.
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginResponses, AddrInfo, Type, StringTmp);
			// 再转发协议至LoginServer.
			// SIMPLE_SERVER_SEND 宏意为 是监听服务器,而且调用监听端口,往其他远端转发.
			// 原理上,是一个客户端接收到服务器协议消息, 然后传递到另一台服务器.
			SIMPLE_SERVER_SEND(LoginServer, SP_LoginResponses, AddrInfo, Type, StringTmp);

			UE_LOG(LogMMOARPGLoginServer, Display, TEXT("[SP_LoginResponses]"));

			break;
		}
	
	}

}

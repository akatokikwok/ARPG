#include "MMOARPGGateRobot.h"
#include "Protocol/HallProtocol.h"
#include "UObject/SimpleController.h"
#include "Global/SimpleNetGlobalInfo.h"

FMMOARPGGateRobot::FMMOARPGGateRobot()
	: GateClient(nullptr)
{

}

FMMOARPGGateRobot::~FMMOARPGGateRobot()
{
	if (GateClient) {
		FSimpleNetManage::Destroy(GateClient);
		GateClient = nullptr;
	}
}

void FMMOARPGGateRobot::Init(const FString& Host, const int32 Port)
{
	FSimpleNetGlobalInfo::Get()->Init();
	GateClient = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);

	GateClient->NetManageMsgDelegate.BindRaw(this, &FMMOARPGGateRobot::LinkServerInfo);// 绑1个客户端往DS发命令的回调.

	if (!GateClient->Init(Host, Port)) {
		delete GateClient;
		GateClient = NULL;
	}

	GateClient->GetController()->RecvDelegate.AddLambda([&](uint32 ProtocolNumber, FSimpleChannel* Channel) ->void {
		RecvProtocol(ProtocolNumber, Channel);
	});

}

void FMMOARPGGateRobot::Tick(float DeltaTime)
{
	if (GateClient) {
		GateClient->Tick(DeltaTime);
	}
}

/// 发这个协议是为了向网关注册玩家id
void FMMOARPGGateRobot::Run_CharacterAppearanceRequests()
{
	/* 仅测试用的假数据.*/
	int32 ID = 1;
	// 扮演客户端 向服务器发送: 玩家外貌协议.
	SIMPLE_CLIENT_SEND(GateClient, SP_CharacterAppearanceRequests, ID);
}

/// 发这个协议是为了 登录,在登录到的DS服务器注册1个玩家.
void FMMOARPGGateRobot::Run_LoginToDSServerRequests()
{
	/* 仅测试用的假数据.*/
	int32 ID = 1;
	int32 InSlotID = 0;// 第4个存档插槽.
	// 扮演客户端 向CS服务器发送命令; 中心服务器上注册1个玩家.
	SIMPLE_CLIENT_SEND(GateClient, SP_LoginToDSServerRequests, ID, InSlotID);
}

/** 握手回调: 客户端往 更高一层的Gate/DS. */
void FMMOARPGGateRobot::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS) {
		Run_CharacterAppearanceRequests();// 先构造客户端样貌假数据 发给上层服务器.
	}
}

void FMMOARPGGateRobot::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber) 
	{
		/** 接收 玩家造型. */
		case SP_CharacterAppearanceResponses:
		{
			FString CharacterJson;
			SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, CharacterJson);

			if (!CharacterJson.IsEmpty()) {
				Run_LoginToDSServerRequests();// 收到外貌响应后才允许 往服务器发送 登录DS请求.
			}
			break;
		}

		/** 接收 登录DS服务器. */
		case SP_LoginToDSServerResponses:
		{
			FSimpleAddr Addr;// 里面存有IP和端口.
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, Addr);
			
			FString DSAddrString = FSimpleNetManage::GetAddrString(Addr);
			StartDelegate.ExecuteIfBound();// 登录DS后 就执行代理(目前绑定AMMOARPGPlayerCharacter::FlushKneadingRequest   是刷新登录人物样貌.).
			break;
		}
	}
}



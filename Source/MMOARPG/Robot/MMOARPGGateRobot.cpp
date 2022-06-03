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
}

void FMMOARPGGateRobot::Tick(float DeltaTime)
{
	if (GateClient) {
		GateClient->Tick(DeltaTime);
	}
}

/** 客户端往DS服务器发命令 的回调. */
void FMMOARPGGateRobot::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	

	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS) {
		
	}
}

void FMMOARPGGateRobot::RunRobot()
{
	int32 ID = 1;
	int32 InSlotID = 3;// 第4个存档插槽.

	/// 想登录到网关,中心服务器上必须要注册1个玩家.

	// 扮演客户端 向CS服务器发送命令; 中心服务器上注册1个玩家.
	SIMPLE_CLIENT_SEND(GateClient, SP_LoginToDSServerRequests, ID, InSlotID);
}

void FMMOARPGGateRobot::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber) 
	{
		/** 登录DS服务器. */
		case SP_LoginToDSServerResponses:
		{
			FSimpleAddr Addr;// 里面存有IP和端口.
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, Addr);
			FString DSAddrString = FSimpleNetManage::GetAddrString(Addr);


		}

	}


}



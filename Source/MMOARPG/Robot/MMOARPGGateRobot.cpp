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

	GateClient->NetManageMsgDelegate.BindRaw(this, &FMMOARPGGateRobot::LinkServerInfo);

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

/** 没有连上然后通知的回调. */
void FMMOARPGGateRobot::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{

}

void FMMOARPGGateRobot::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{

}



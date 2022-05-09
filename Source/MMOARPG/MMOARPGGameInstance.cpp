// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGGameInstance.h"
#include "../Plugins/SimpleNetChannel/Source/SimpleNetChannel/Public/Global/SimpleNetGlobalInfo.h"
#include "ThreadManage.h"

void UMMOARPGGameInstance::Init()
{
	Super::Init();

}

void UMMOARPGGameInstance::Tick(float DeltaTime)
{
// 	Super::Tick(DeltaTime);

	if (Client) {
		Client->Tick(DeltaTime);
		// 手动执行让这个协程运行起来.
		GThread::Get()->Tick(DeltaTime);
	}
}

TStatId UMMOARPGGameInstance::GetStatId() const
{

	return TStatId();
}

void UMMOARPGGameInstance::Shutdown()
{
	Super::Shutdown();

	if (Client) {
		FSimpleNetManage::Destroy(Client);  
		GThread::Destroy();// 同时把协程也干掉.
	}
}

void UMMOARPGGameInstance::CreateClient()
{
	// 初始化全局表.
	FSimpleNetGlobalInfo::Get()->Init();
	// 创建客户端对象.
	Client = 
		FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	//
}

void UMMOARPGGameInstance::LinkServer()
{
	if (Client != nullptr) {
		if (!Client->Init()) {
			delete Client;
			Client = nullptr;
		}
	}
}

FSimpleNetManage* UMMOARPGGameInstance::GetClient()
{
	return Client;
}

FMMOARPGUserData& UMMOARPGGameInstance::GetUserData()
{
	return UserData;
}

FMMOARPGGateStatus UMMOARPGGameInstance::GetGateStatus()
{
	return GateStatus;
}

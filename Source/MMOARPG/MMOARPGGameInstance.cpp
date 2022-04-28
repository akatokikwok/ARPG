// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGGameInstance.h"
#include "../Plugins/SimpleNetChannel/Source/SimpleNetChannel/Public/Global/SimpleNetGlobalInfo.h"

void UMMOARPGGameInstance::Init()
{
	Super::Init();

}

void UMMOARPGGameInstance::Tick(float DeltaTime)
{
// 	Super::Tick(DeltaTime);

	if (Client) {
		Client->Tick(DeltaTime);

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
	}
}

void UMMOARPGGameInstance::CreateClient()
{
	// ��ʼ��ȫ�ֱ�.
	FSimpleNetGlobalInfo::Get()->Init();
	// �����ͻ��˶���.
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

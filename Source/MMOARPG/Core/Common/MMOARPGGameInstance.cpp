﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGGameInstance.h"
#include "../Plugins/SimpleNetChannel/Source/SimpleNetChannel/Public/Global/SimpleNetGlobalInfo.h"
#include "ThreadManage.h"
#include "SimpleAdvancedAnimationBPLibrary.h"
#include "Tickable/MMOARPGTickable.h"

void UMMOARPGGameInstance::Init()
{
	Super::Init();

	// 启动使用一个可以操控协程来Tick的单例. 单例的协程版本Tick就会启动
	FMMOARPGTickable::Get();
}

void UMMOARPGGameInstance::Tick(float DeltaTime)
{
// 	Super::Tick(DeltaTime);

	if (Client) {
		Client->Tick(DeltaTime);
		
	}
	/**
	 * 如果在GameInstance::Tick里使用协程,则仅有1个客户端生效,多个不行
	 */

// 	// 手动执行让这个协程Tick起来.
// 	GThread::Get()->Tick(DeltaTime);
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
	FMMOARPGTickable::Destroy();// 销毁操控协程Tick的单例

	GThread::Destroy();// 同时把协程也干掉.

	// GINS退出的时候也要销毁高级动画插件里的单例.
	USimpleAdvancedAnimationBPLibrary::Destroy();// 销毁高级动画架构层单例.
}

void UMMOARPGGameInstance::CreateClient()
{
	if (Client == nullptr) {// 仅当空才创建,已存在客户端则不二次创建.
		// 初始化全局表.
		FSimpleNetGlobalInfo::Get()->Init();
		// 创建客户端对象.
		Client =
			FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	}
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

void UMMOARPGGameInstance::LinkServer(const FSimpleAddr& InAddr)
{
	if (Client) {
		if (!Client->Init(InAddr)) {
			delete Client;
			Client = NULL;
		}
	}
}

void UMMOARPGGameInstance::LinkServer(const TCHAR* InIP, uint32 InPort)
{
	if (Client) {
		if (!Client->Init(InIP, InPort)) {
			delete Client;
			Client = NULL;
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

FMMOARPGGateStatus& UMMOARPGGameInstance::GetGateStatus()
{
	return GateStatus;
}

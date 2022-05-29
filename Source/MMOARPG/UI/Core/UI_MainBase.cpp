// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_MainBase.h"
#include "../Common/UI_Print.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../../MMOAPRGMacroType.h"
#include "../../MMOARPGGameInstance.h"

// void UUI_MainBase::LinkServer(const FSimpleAddr& InAddr)
// {
// 	//创建客户端
// 	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
// 	{
// 		InGameInstance->CreateClient();
// 		if (InGameInstance->GetClient())
// 		{
// 			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_MainBase::LinkServerInfo);
// 
// 			InGameInstance->LinkServer(InAddr);
// 
// 			BindClientRcv();
// 		}
// 	}
// }
// 
// void UUI_MainBase::LinkServer()
// {
// 	//创建客户端
// 	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
// 	{
// 		InGameInstance->CreateClient();
// 		if (InGameInstance->GetClient())
// 		{
// 			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_MainBase::LinkServerInfo);
// 
// 			InGameInstance->LinkServer();
// 
// 			BindClientRcv();
// 		}
// 	}
// }


void UUI_MainBase::NativeConstruct()
{
	Super::NativeConstruct();

	/** 创建客户端 */
	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		InGameIns->CreateClient();// 创建客户端.

		if (InGameIns->GetClient() != nullptr) {
			// CharacterRequests是从这里借助这个代理发送出去的; 为网络消息协议绑定回调.
			InGameIns->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_MainBase::LinkServerInfo);
			// 			// 利用客户端直接实行初始化.
			// 			InGameIns->GetClient()->Init(InGameIns->GetGateStatus().GateServerAddrInfo.Addr);

			InGameIns->LinkServer();// 链接至服务器.

			// 在构造的时候 就循环创建与绑定.
			BindClientRcv();
		}
	}

}

void UUI_MainBase::NativeDestruct()
{
	Super::NativeDestruct();

	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController()) {
			// 销毁的时候记得移除客户端网络控制器里的代理.
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(mRecvDelegate);
		}
	}
}

void UUI_MainBase::PrintLog(const FString& InMsg)
{
	// 调用重载PrintLog.
	PrintLog(FText::FromString(InMsg));
}

void UUI_MainBase::PrintLog(const FText& InMsg)
{
	// 播放打印用的字体 动画.
	UI_Print->PlayTextAnim();
	// 设置文字并打印消息.
	UI_Print->SetText(InMsg);
}

void UUI_MainBase::BindClientRcv()
{
	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameIns->GetClient() && InGameIns->GetClient()->GetController()) {/// 正常情况.
			// 给客户端网络控制器的委托绑定效果.
			mRecvDelegate = InGameIns->GetClient()->GetController()->RecvDelegate.AddLambda(
				[&](uint32 ProtocolNumber, FSimpleChannel* Channel) ->void {
					this->RecvProtocol(ProtocolNumber, Channel);
			});
		}
		else {/// 有Gameinstance,但没客户端的情况.
			GThread::Get()->GetCoroutines().BindLambda(// 借助协程的形式.
				0.5f, [&]() {
					this->BindClientRcv();// 递归进来又一遍执行自己.
			});
		}

	}
	/// 还未产生GameInstance的情况.
	else {
		GThread::Get()->GetCoroutines().BindLambda(// 借助协程的形式.
			0.5f, [&]() {
				this->BindClientRcv();// 递归进来又一遍执行自己.
		});
	}

	// 	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
	// 		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController()) {
	// 			mRecvDelegate = InGameInstance->GetClient()->GetController()->RecvDelegate.AddLambda(
	// 				[&](uint32 ProtocolNumber, FSimpleChannel* Channel) {
	// 					this->RecvProtocol(ProtocolNumber, Channel);
	// 				});
	// 		}
	// 		else {
	// 			GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
	// 		}
	// 	}
	// 	else {
	// 		GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
	// 	}

}

void UUI_MainBase::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{

}
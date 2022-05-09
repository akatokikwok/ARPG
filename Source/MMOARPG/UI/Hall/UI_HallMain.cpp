// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HallMain.h"
#include "../../MMOARPGGameInstance.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"

void UUI_HallMain::NativeConstruct()
{
	Super::NativeConstruct();
	/** 1.创建客户端 */
	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameIns->GetClient() != nullptr) {
			// 利用客户端直接实行初始化.
			InGameIns->GetClient()->Init(InGameIns->GetGateStatus().GateServerAddrInfo.Addr);
			// 在构造的时候 就循环创建与绑定.
			BindClientRcv();
		}
	}

}

void UUI_HallMain::NativeDestruct()
{
	Super::NativeDestruct();

	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameIns->GetClient() && InGameIns->GetClient()->GetController()) {
			// 销毁的时候记得移除客户端网络控制器里的代理.
			InGameIns->GetClient()->GetController()->RecvDelegate.Remove(mRecvDelegate);
		}
	}
}

void UUI_HallMain::PrintLog(const FString& InMsg)
{
	// 调用重载PrintLog.
	PrintLog(FText::FromString(InMsg));
}

void UUI_HallMain::PrintLog(const FText& InMsg)
{
	// 播放打印用的字体 动画.
	UI_Print->PlayTextAnim();

	// 设置文字并打印消息.
	UI_Print->SetText(InMsg);
}

void UUI_HallMain::BindClientRcv()
{
	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		// 正常情况.
		if (InGameIns->GetClient() && InGameIns->GetClient()->GetController()) {
			// 给客户端网络控制器的委托绑定效果.
			mRecvDelegate = InGameIns->GetClient()->GetController()->RecvDelegate.AddLambda(
				[&](uint32 ProtocolNumber, FSimpleChannel* Channel) ->void {
					// 
					this->RecvProtocol(ProtocolNumber, Channel);
				}
			);
		}
		// 有Gameinstance,但没客户端的情况.
		else {
			// 借助协程的形式.
			GThread::Get()->GetCoroutines().BindLambda(
				0.5f, [&]() {
					this->BindClientRcv();// 递归进来又一遍执行自己.
				}
			);
		}
	}
	// 还未产生GameInstance的情况.
	else {
		// 借助协程的形式.
		GThread::Get()->GetCoroutines().BindLambda(
			0.5f, [&]() {
				this->BindClientRcv();// 递归进来又一遍执行自己.
			}
		);
	}
}

void UUI_HallMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{

}

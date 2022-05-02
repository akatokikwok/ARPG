// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_LoginMain.h"
#include "SimpleNetManage.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "Components/TextBlock.h"
#include "../Core/UI_Base.h"
#include "UI_Login.h"
#include "Protocol/LoginProtocol.h"
#include "SimpleProtocolsDefinition.h"
#include "../../MMOAPRGMacroType.h"
#include "MMOARPGType.h"

void UUI_LoginMain::NativeConstruct()
{
	Super::NativeConstruct();
	
	UI_Login->SetParents(this);

	/** 1.创建客户端 */
	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		InGameIns->CreateClient();
		if (InGameIns->GetClient() != nullptr) {
			// 客户端向服务端握手的时候,采用激活这个NetManageMsgDelegate代理.
			InGameIns->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_LoginMain::Callback_LinkServerInfo);
			// 再让GINS链接至服务器.
			InGameIns->LinkServer();

			BindClientRcv();// 在构造的时候 就循环创建与绑定.
		}
	}
}

void UUI_LoginMain::NativeDestruct()
{
	Super::NativeDestruct();

	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameIns->GetClient() && InGameIns->GetClient()->GetController()) {
			// 销毁的时候记得移除客户端网络控制器里的代理.
			InGameIns->GetClient()->GetController()->RecvDelegate.Remove(mRecvDelegate);
		}
	}
}

void UUI_LoginMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber) {
		case SP_LoginResponses : 
		{
			FString StringTmp;// 接受协议后有值的Json String.
			ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;
			// 接收响应协议里的数据.
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginResponses, Type, StringTmp, GateStatus);

			// 依据不通的登录状态分类.
			switch (Type) {
				case LOGIN_DB_SERVER_ERROR:
					PrintLog(TEXT("服务器错误."));
					break;
				case LOGIN_SUCCESS:// 当且仅当登录成功.
				{
					// 把有值的JSON数据解析成 用户数据.
					if (UMMOARPGGameInstance* InGINS = GetGameInstance<UMMOARPGGameInstance>()) {
						if (StringTmp != TEXT("[]")) {
							NetDataAnalysis::StringToUserData(StringTmp, InGINS->GetUserData());
						}
					}
					PrintLog(TEXT("登录成功."));
					break;
				}
					
				case LOGIN_ACCOUNT_WRONG:
					PrintLog(TEXT("账户不存在."));
					break;
				case LOGIN_PASSWORD_WRONG:
					PrintLog(TEXT("密码验证失败."));
					break;
			}
			
			
			

			break;
		}
		
		
	}
}

void UUI_LoginMain::BindClientRcv()
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

void UUI_LoginMain::SignIn(FString& InAccount, FString& InPassword)
{
	/** 使用在专用头文件里封装好的宏,这个宏负责发送协议,到Login服务器. */
	SEND_DATA(SP_LoginRequests, InAccount, InPassword);
	
}

void UUI_LoginMain::Register()
{

}

void UUI_LoginMain::Callback_LinkServerInfo(ESimpleNetErrorType InErrorType, const FString& InMsg)
{
	// 握手成功.
	if (InErrorType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS) {
		UI_LinkWidget->SetVisibility(ESlateVisibility::Collapsed);// 隐藏此Widget.
	}
}

void UUI_LoginMain::PrintLog(const FString& InMsg)
{
	// 播放动画.


	// 设置文字并打印消息.
	MsgLog->SetText(FText::FromString(InMsg));
}

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
#include "Kismet/GameplayStatics.h"

void UUI_LoginMain::NativeConstruct()
{
	Super::NativeConstruct();
	// 播淡入淡出动画.
	UUI_Base::PlayWidgetAnim(TEXT("LoginIn"));// 名为LoginIn的动画在蓝图里指定.

	// 为登录界面设置持有者.
	UI_Login->SetParents(this);

	UUI_MainBase::LinkServer();// 链接至服务器并循环创建绑定.

// 	/** 1.创建客户端 */
// 	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
// 		InGameIns->CreateClient();
// 		if (InGameIns->GetClient() != nullptr) {
// 			// 客户端向服务端握手的时候,采用激活这个NetManageMsgDelegate代理.
// 			InGameIns->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_LoginMain::LinkServerInfo);
// 			// 再让GINS链接至服务器.
// 			InGameIns->LinkServer();
// 
// 			BindClientRcv();// 在构造的时候 就循环创建与绑定.
// 		}
// 	}

	// 先解密一次; 读取账号密码; 
	if (!UI_Login->DecryptionFromLocal(FPaths::ProjectDir() / TEXT("User"))) {
		PrintLog(TEXT("No Account Detected."));
	}

}

void UUI_LoginMain::NativeDestruct()
{
	Super::NativeDestruct();
 	
}

void UUI_LoginMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber) {
		case SP_LoginResponses:
		{
			FString StringTmp;// 接受协议后有值的Json String.
			ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;
			FMMOARPGGateStatus GateStatus;// 协议数据里的网关状态.

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
						/* 网关连接数为 - 1, 即爆满的情况. */
						if (GateStatus.GateConnectionNum == INDEX_NONE) {
							// 等待提示.
							PrintLog(TEXT("At Present,the number of Gate Connections is full, and we try to reconnect again."));
						}
						/* 网关连接数为合理情况, 即未爆满的情况. */
						else {
							InGINS->GetGateStatus() = GateStatus;// 刷新一次GINS里的网关.

							// 加密密码至本地.
							if (!UI_Login->EncryptionToLocal(FPaths::ProjectDir() / TEXT("User"))) {
								PrintLog(TEXT("Password storage failed, 密码储存失败."));
							}
							else {
								PrintLog(TEXT("Login Success, 密码储存成功."));
							}
							// 登陆成功播一次淡出动画.
							UUI_Base::PlayWidgetAnim(TEXT("LoginOut"));// 名为LoginOut的动画在蓝图里指定.

							// 播完动画后就关闭Login服务器, 接入下一个服务器:角色创建大厅.
							if (InGINS->GetClient() && InGINS->GetClient()->GetChannel()) {
								InGINS->GetClient()->GetChannel()->DestroySelf();// 关闭客户端即不再给登录服务器发消息.
							}
							//协程, 延迟2.0s 打开另一张地图.
							GThread::Get()->GetCoroutines().BindLambda(2.0f, 
								[&]() ->void {
									UGameplayStatics::OpenLevel(GetWorld(), TEXT("HallMap"));
							});
						}
					}
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

void UUI_LoginMain::SignIn(FString& InAccount, FString& InPassword)
{
	/** 使用在专用头文件里封装好的宏,这个宏负责发送协议,到Login服务器. */
	SEND_DATA(SP_LoginRequests, InAccount, InPassword);

}

void UUI_LoginMain::Register()
{

}

void UUI_LoginMain::LinkServerInfo(ESimpleNetErrorType InErrorType, const FString& InMsg)
{
	// 握手成功.
	if (InErrorType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS) {
		UI_LinkWidget->SetVisibility(ESlateVisibility::Collapsed);// 隐藏此Widget.
	}
}

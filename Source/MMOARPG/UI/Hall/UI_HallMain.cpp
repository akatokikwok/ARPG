// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HallMain.h"
#include "../../MMOARPGGameInstance.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "Protocol/HallProtocol.h"
#include "../../MMOAPRGMacroType.h"
#include "../../Core/Hall/HallPlayerState.h"

void UUI_HallMain::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 将主面板设置为各子层级UI的持有者.
	UI_RenameCreate->SetParents(this);
	UI_CharacterCreatePanel->SetParents(this);

	/** 1.创建客户端 */
	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameIns->GetClient() != nullptr) {
			// CharacterRequests是从这里借助这个代理发送出去的; 为网络消息协议绑定回调.
			InGameIns->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_HallMain::Callback_LinkServerInfo);
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

void UUI_HallMain::PlayRenameIn()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameIn"));
}

void UUI_HallMain::PlayRenameOut()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameOut"));
}

/** 负责还原 Create面板. */
void UUI_HallMain::ResetCharacterCreatePanel()
{
	UI_CharacterCreatePanel->CreateCharacterButtons();// 把create面板的4个加号再补回来.
}

void UUI_HallMain::SpawnRecentCharacter()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>()) {
		if (FMMOARPGCharacterAppearance* InRecentCAData = InState->GetRecentCharacter()) {// 先拿取PS里最近的人物存档.
			UI_CharacterCreatePanel->SpawnCharacter(InRecentCAData);
		}
	}
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
	switch (ProtocolNumber) {
		case SP_CharacterAppearanceResponses :// 客户端收到角色形象 Response协议.
		{
			FString CharacterJson;// 玩家形象Json形式的数据源.
			SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, CharacterJson);// 得到客户端发送到服务器的账号(以json数据形式返回).

			/** 投射存档数据到UI, 投射存档人物至场景. */
			if (!CharacterJson.IsEmpty()) {// json数据源判空.
				if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>()) {
					// 解析Json数据源到PS里.
					NetDataAnalysis::StringToCharacterAppearances(CharacterJson, InState->GetCharacterAppearance());
					// 初始化所有关联数据包的 加号外观.(默认设定是4个).
					UI_CharacterCreatePanel->InitCharacterButtons(InState->GetCharacterAppearance());
					// 附带生成1个最新玩耍过的存档人物.
					SpawnRecentCharacter();
					// 高亮UI.

				}
			}
			break;
		}
	}
}

/** 发送Request协议: 玩家形象. */
void UUI_HallMain::Callback_LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS) {
		// 握手成功就向服务器发送角色形象Requests; 发送角色形象请求协议(4号协议.)
		if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
			SEND_DATA(SP_CharacterAppearanceRequests, InGameInstance->GetUserData().ID);// 发出去这个请求协议, 让网关去接收.
		}
	}
}

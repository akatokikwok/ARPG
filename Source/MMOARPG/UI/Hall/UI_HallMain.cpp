﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HallMain.h"
#include "../../MMOARPGGameInstance.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "Protocol/HallProtocol.h"
#include "../../MMOAPRGMacroType.h"
#include "../../Core/Hall/HallPlayerState.h"
#include "MMOARPGType.h"

#define LOCTEXT_NAMESPACE "UUI_HallMain"// 用于本地化.

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
	// 把create面板的4个加号再补回来.
	UI_CharacterCreatePanel->CreateCharacterButtons();

	// 除去UI,还需要恢复生成最近的舞台人物.
	SpawnRecentCharacter();

	// 使最近存档关联的槽位按钮高亮.
	HighlightDefaultSelection();
}

/** 生成最近CA存档关联的舞台人物. */
void UUI_HallMain::SpawnRecentCharacter()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>()) {
		if (FMMOARPGCharacterAppearance* InRecentCAData = InState->GetRecentCharacter()) {// 先拿取PS里最近的人物存档.
			UI_CharacterCreatePanel->SpawnCharacter(InRecentCAData);// 生成最近CA存档关联的舞台人物.
		}
	}
}

/** 使最近存档关联的槽位按钮高亮. */
void UUI_HallMain::HighlightDefaultSelection()
{
	if (AHallPlayerState* PS = GetPlayerState<AHallPlayerState>()) {
		if (FMMOARPGCharacterAppearance* InRecentCAData = PS->GetRecentCharacter()) {// 先拿取PS里最近的人物存档.
			UI_CharacterCreatePanel->HighlightSelection(InRecentCAData->SlotPosition);// 使最近存档关联的槽位按钮高亮.
		}
	}
}

/** 核验角色命名 */
void UUI_HallMain::CheckRename(FString& InCharacterName)
{
	// 向服务端发送核验名称的请求
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		SEND_DATA(SP_CheckCharacterNameRequests, InGameInstance->GetUserData().ID, InCharacterName);
	}
}

/** 向服务端发送创建角色的请求. */
void UUI_HallMain::CreateCharacter(const FMMOARPGCharacterAppearance& InCA)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		// 指定CA压缩成Json.
		FString CAJson;
		NetDataAnalysis::CharacterAppearancesToString(InCA, CAJson);

		SEND_DATA(SP_CreateCharacterRequests, InGameInstance->GetUserData().ID, CAJson);
	}
}

void UUI_HallMain::SetSlotPosition(const int32 InSlotIndex)
{
	UI_RenameCreate->SetSlotPosition(InSlotIndex);
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
		
		/** 收到来自db的 玩家造型的回复协议. */
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
					// 使高亮 最近CA存档的 UI button.
					HighlightDefaultSelection();
				}
			}
			break;
		}

		/** 收到来自db的 核验输入名称 的回复协议. */
		case SP_CheckCharacterNameResponses :
		{
			ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;// 核验类型.
			SIMPLE_PROTOCOLS_RECEIVE(SP_CheckCharacterNameResponses, CheckNameType);// 拿取到服务器返回的舞台人物名字核验类型.

			PrintLogByCheckName(CheckNameType);// 根据核验结果分别打印提示.
			break;
		}

		/** 收到来自db的 创建舞台人物 的回复协议. */
		case SP_CreateCharacterResponses:
		{
			ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;// 核验结果
			bool bCreateCharacter = false;// 创建信号.
			SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterResponses, CheckNameType, bCreateCharacter);// 收取服务端回复的2个属性数据.

			if (bCreateCharacter == true) {
				PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_SUCCESSFULLY", "created successfully."));

			}
			else {
				PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_FAIL", "created fail."));
				// 延迟1.5秒打印核验结果.
				GThread::Get()->GetCoroutines().BindLambda(1.5f, [=]() { PrintLogByCheckName(CheckNameType); });
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

/** 根据核验结果分别打印提示. */
void UUI_HallMain::PrintLogByCheckName(ECheckNameType InCheckNameType)
{
	switch (InCheckNameType) {
		case UNKNOWN_ERROR:
			PrintLog(LOCTEXT("CHECK_NAME_UNKNOWN_ERROR", "The server encountered an unknown error."));
			break;
		case NAME_NOT_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_NAME_NOT_EXIST", "The name is Invalid"));
			break;
		case SERVER_NOT_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_SERVER_NOT_EXIST", "Server error."));
			break;
		case NAME_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_NAME_EXIST", "The name has been registered."));
			break;
	}
}

#undef LOCTEXT_NAMESPACE// 用于本地化.

#include "UI_HallMain.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../Common/UI_Print.h"
#include "Element/UI_CharacterCreatePanel.h"
#include "Element/UI_RenameCreate.h"
#include "Element\KneadFace\UI_EditorCharacter.h"
#include "Protocol/HallProtocol.h"
#include "../../Core/Hall/HallPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../../MMOAPRGMacroType.h"
#include "..\..\Core\Hall\HallPawn.h"

#define LOCTEXT_NAMESPACE "UUI_HallMain"// 用于本地化.

void UUI_HallMain::NativeConstruct()
{
	Super::NativeConstruct();
	
	HallMainIn();// 播亮屏动画.

	// 将主面板设置为各子层级UI的持有者.
	UI_RenameCreate->SetParents(this);
	UI_CharacterCreatePanel->SetParents(this);
	UI_EditorCharacter->SetParents(this);

	if (UMMOARPGGameInstance* InGIns = GetGameInstance<UMMOARPGGameInstance>()) {
		UUI_MainBase::LinkServer(InGIns->GetGateStatus().GateServerAddrInfo.Addr);// 链接至GINS里的网关服务器地址 并循环创建绑定.
	}
	
}

void UUI_HallMain::NativeDestruct()
{
	Super::NativeDestruct();

// 	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
// 		if (InGameIns->GetClient() && InGameIns->GetClient()->GetController()) {
// 			// 销毁的时候记得移除客户端网络控制器里的代理.
// 			InGameIns->GetClient()->GetController()->RecvDelegate.Remove(mRecvDelegate);
// 		}
// 	}
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
void UUI_HallMain::ResetCharacterCreatePanel(bool bSpawnNewCharacter)
{
	// 把create面板的4个加号再补回来.
	UI_CharacterCreatePanel->CreateCharacterButtons();

	if (bSpawnNewCharacter == true) {
		// 除去UI,还需要恢复生成最近的舞台人物.
		SpawnRecentCharacter();
	}

	// 使最近存档关联的槽位按钮高亮.
	HighlightDefaultSelection();
}

/** 生成最近CA存档关联的舞台人物. */
void UUI_HallMain::SpawnRecentCharacter()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>()) {
		if (FMMOARPGCharacterAppearance* InRecentCAData = InState->GetRecentCharacter()) {// 先拿取PS里最近的人物存档.
			UI_CharacterCreatePanel->SpawnCharacter(InRecentCAData);// 生成最近CA存档关联的舞台人物.

			SetEditCharacter(InRecentCAData);
		}
		else {
			SetEditCharacter(nullptr);
			DestroyCharacter();
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

void UUI_HallMain::DeleteCharacter(int32 InSlot)
{
	if (InSlot >= 0 && InSlot < 4) {
		if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
			// 发送删除指定槽号的CA 请求.
			SEND_DATA(SP_DeleteCharacterRequests, InGameInstance->GetUserData().ID, InSlot);
		}
	}
}

void UUI_HallMain::SetEditCharacter(const FMMOARPGCharacterAppearance* InCA)
{
	if (InCA) {
		UI_EditorCharacter->SetCharacterName(FText::FromString(InCA->Name));
		UI_EditorCharacter->SetSlotID(InCA->SlotPosition);
	}
	else {
		UI_EditorCharacter->SetCharacterName(FText::FromString(""));
		UI_EditorCharacter->SetSlotID(INDEX_NONE);
	}
}

void UUI_HallMain::DestroyCharacter()
{
	//删除刚刚角色
	if (AHallPawn* InPawn = GetPawn<AHallPawn>()) {
		if (InPawn->CharacterStage) {
			InPawn->CharacterStage->Destroy();
			InPawn->CharacterStage = nullptr;
		}
	}
}

/** 向服务端发送跳转至DS的请求. */
void UUI_HallMain::JumpDSServer(int32 InSlotID)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		SEND_DATA(SP_LoginToDSServerRequests, InGameInstance->GetUserData().ID, InSlotID);// InSlotID是具体的哪个槽号的人物.
	}
}

/** 循环绑定. */
//  void UUI_HallMain::BindClientRcv()
//  {
//  	if (UMMOARPGGameInstance* InGameIns = GetGameInstance<UMMOARPGGameInstance>()) {
//  		// 正常情况.
//  		if (InGameIns->GetClient() && InGameIns->GetClient()->GetController()) {
//  			// 给客户端网络控制器的委托绑定效果.
//  			mRecvDelegate = InGameIns->GetClient()->GetController()->RecvDelegate.AddLambda(
//  				[&](uint32 ProtocolNumber, FSimpleChannel* Channel) ->void {
//  					// 
//  					this->RecvProtocol(ProtocolNumber, Channel);
//  				}
//  			);
//  		}
//  		// 有Gameinstance,但没客户端的情况.
//  		else {
//  			// 借助协程的形式.
//  			GThread::Get()->GetCoroutines().BindLambda(
//  				0.5f, [&]() {
//  					this->BindClientRcv();// 递归进来又一遍执行自己.
//  				}
//  			);
//  		}
//  	}
//  	// 还未产生GameInstance的情况.
//  	else {
//  		// 借助协程的形式.
//  		GThread::Get()->GetCoroutines().BindLambda(
//  			0.5f, [&]() {
//  				this->BindClientRcv();// 递归进来又一遍执行自己.
//  			}
//  		);
//  	}
//  }

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
			GEngine->AddOnScreenDebugMessage(-1,10.0f, FColor::Green, TEXT("客户端收到了协议SP_CreateCharacterResponses"));

			ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;// 核验结果
			bool bCreateCharacter = false;// 创建信号.
			FString JsonString_CA;
			SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterResponses, CheckNameType, bCreateCharacter, JsonString_CA);// 收取服务端回复的3个属性数据.

			if (bCreateCharacter == true) {
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("bCreateCharacter == true"));
				PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_SUCCESSFULLY", "created successfully."));
				// 从JSON数据源中解析出CA存档.
				FMMOARPGCharacterAppearance InCA;
				NetDataAnalysis::StringToCharacterAppearances(JsonString_CA, InCA);
				// 
				if (AHallPlayerState* InPS = GetPlayerState<AHallPlayerState>()) {
					InPS->AddCharacterCA(InCA);
					
					PlayRenameOut();// 播放Rename控件的淡出动画.
					ResetCharacterCreatePanel(false);// 还原Create控件(这里设置为暂不生成人物).

					SetEditCharacter(&InCA);// 设定正在编辑的槽孔.
				}
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("bCreateCharacter == false"));
				PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_FAIL", "created fail."));
				// 延迟1.5秒打印核验结果.
				GThread::Get()->GetCoroutines().BindLambda(1.5f, [=]() { PrintLogByCheckName(CheckNameType); });
			}
			break;
		}

		/** 删除角色. */
		case SP_DeleteCharacterResponses:
		{
			int32 UserID = INDEX_NONE;// 用户ID
			FSimpleAddrInfo AddrInfo;
			int32 SlotID = INDEX_NONE;// 要删除的槽号.
			int32 SuccessDeleteCount = 0;// 删除成功的计数.
			SIMPLE_PROTOCOLS_RECEIVE(SP_DeleteCharacterResponses, UserID, SlotID, SuccessDeleteCount);

			if (SuccessDeleteCount > 2) {/* 满足删除技术成功次数大于2,才满足完整地删除流程. */
				if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>()) {

					InState->RemoveCharacterAppearanceBySlot(SlotID);// 先删除PS里的存档数据.
					ResetCharacterCreatePanel(true);// 再还原Create面板外观.
				}
				PrintLog(LOCTEXT("DELETE_CHARACTER_SUCCESS", "The role deletion is successful, and the change operation is irreversible."));
			}
			else {
				PrintLog(LOCTEXT("DELETE_CHARACTER_ERROR", "Failed to delete the role. Please check if the role exists."));
			}
		}

		/** 登录DS服务器. */
		case SP_LoginToDSServerResponses :
		{
			FSimpleAddr Addr;// 里面存有IP和端口.
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, Addr);
			FString DSAddrString = FSimpleNetManage::GetAddrString(Addr);

			HallMainOut();// 播熄屏动画.

			//协程
			GThread::Get()->GetCoroutines().BindLambda(1.f, [=]() {
				UGameplayStatics::OpenLevel(GetWorld(), *DSAddrString);
			});
			break;
		}

	}
}

/** 发送Request协议: 玩家形象. */
void UUI_HallMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
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
			PrintLog(LOCTEXT("CHECK_NAME_NAME_NOT_EXIST", "The name is Valid."));
			break;
		case SERVER_NOT_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_SERVER_NOT_EXIST", "Server error."));
			break;
		case NAME_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_NAME_EXIST", "The name has been registered."));
			break;
	}
}

// 播放主界面淡入动画.
void UUI_HallMain::HallMainIn()
{
	PlayWidgetAnim(TEXT("HallMainIn"));
}

// 播放主界面淡出动画.
void UUI_HallMain::HallMainOut()
{
	PlayWidgetAnim(TEXT("HallMainOut"));
}

#undef LOCTEXT_NAMESPACE// 用于本地化.

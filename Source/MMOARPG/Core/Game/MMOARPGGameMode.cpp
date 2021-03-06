// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGGameMode.h"
#include "Character/MMOARPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MMOARPGHUD.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../../MMOARPGMacroType.h"
#include "MMOARPGPlayerState.h"
#include "MMOARPGGameState.h"
#include "ThreadManage.h"
#include "Character/MMOARPGPlayerCharacter.h"
#include "Protocol/GameProtocol.h"
#include "Core/MethodUnit.h"
#include "MMOARPGPlayerController.h"
#include <MMOARPGType.h>
#include "MMOARPGTagList.h"// 这个是引擎插件里的
// #include "../../MMOARPGTagList.h"// 这个是项目里的

AMMOARPGGameMode::AMMOARPGGameMode()
{
	HUDClass = AMMOARPGHUD::StaticClass();// HUD暂设为 Game-HUD,游玩时候的HUD.
	PlayerStateClass = AMMOARPGPlayerState::StaticClass();// 注册PS
	GameStateClass = AMMOARPGGameState::StaticClass();// 注册GS
	PlayerControllerClass = AMMOARPGPlayerController::StaticClass();// 注册Controiler.

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMMOARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	/**
	 * 只在服务端执行.
	 * 链接至指定端口号的服务器(可以是Center/DS服务器)并 循环绑定创建客户端接收响应协议的回调RecvProtocol.
	 */
	LinkServer();
}

void AMMOARPGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController()) {
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(mRecvDelegate);
		}
	}
}

void AMMOARPGGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** 玩家登录到DS后会激活的1个接口. */
void AMMOARPGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	// 使用协程,让这一步有时间去准备.延迟0.5s是因为DS准备好之后,要给予客户端一些时间执行同步.
	GThread::Get()->GetCoroutines().BindLambda(
		0.5f, [&](APlayerController* InNewController) ->void {
			if (InNewController != nullptr) {
				// to do.

// 				if (AMMOARPGPlayerCharacter* InPawn = InNewController->GetPawn<AMMOARPGPlayerCharacter>()) {// 先拿人
// 					if (AMMOARPGGameState* InGS = GetGameState<AMMOARPGGameState>()) {// 再拿GS
// 						// 把GS里的动画数据解算到Player身上.
// 						if (FCharacterAnimTable* InAnimRowData = InGS->GetCharacterAnimTable(InPawn->GetID())) {
// 							InPawn->AnimTable = InAnimRowData;
// 						}
// 					}
// 				}
			}
		}, NewPlayer
	);
}

void AMMOARPGGameMode::LoginCharacterUpdateKneadingRequest(int32 InUserID)
{
	SEND_DATA(SP_UpdateLoginCharacterInfoRequests, InUserID);// 向CS发送一个刷新登录人物请求.
}

/** 向CS服务器发送 gas人物属性集请求 */
void AMMOARPGGameMode::GetCharacterDataRequests(int32 InUserID, int32 InCharacterID, int32 MMOARPGSlot)
{
	SEND_DATA(SP_GetCharacterDataRequests, InUserID, InCharacterID, MMOARPGSlot);
}

void AMMOARPGGameMode::BindClientRcv()
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController()) {
			mRecvDelegate = InGameInstance->GetClient()->GetController()->RecvDelegate.AddLambda(
				[&](uint32 ProtocolNumber, FSimpleChannel* Channel) {
					this->RecvProtocol(ProtocolNumber, Channel);
				});
		}
		else {
			GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
		}
	}
	else {
		GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
	}
}

void AMMOARPGGameMode::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{

}

void AMMOARPGGameMode::LinkServer()
{
	//创建客户端
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>()) {
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient()) {
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &AMMOARPGGameMode::LinkServerInfo);

			//InGameInstance->LinkServer();
			//
			// 作为测试,11231是中心服务器端口.
			// 即暂用 链接到本机的中心服务器.
			InGameInstance->LinkServer(TEXT("127.0.0.1"), 11231);

			BindClientRcv();
		}
	}
}

/// 当DS接收到来自中心服务器的回复.
void AMMOARPGGameMode::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber) {
		// 刷新登录人物外观回复.
		case SP_UpdateLoginCharacterInfoResponses:
		{
			int32 UserID = INDEX_NONE;
			FString CAJsonString;
			SIMPLE_PROTOCOLS_RECEIVE(SP_UpdateLoginCharacterInfoResponses, UserID, CAJsonString);

			if (UserID != INDEX_NONE && !CAJsonString.IsEmpty()) {

				// 因为每个人物都有1个controller和1个PS,所以这边遍历的时候推荐用Controller而不是Player 
				// 需要一个仿函数去处理.
				MethodUnit::ServerCallAllPlayerController<AMMOARPGPlayerController>(
					GetWorld(),
					[&](AMMOARPGPlayerController* InController) ->MethodUnit::EServerCallType {

						if (AMMOARPGPlayerCharacter* InPlayerCharacter = InController->GetPawn<AMMOARPGPlayerCharacter>()) {//拿人
							if (InPlayerCharacter->GetUserID() == UserID) {// 校验ID

								if (AMMOARPGPlayerState* InPlayerState = InController->GetPlayerState<AMMOARPGPlayerState>()) {// 拿PS
									// 解析外貌流 到PS里存一份.
									NetDataAnalysis::StringToCharacterAppearances(CAJsonString, InPlayerState->GetCA());
									// DS上刷新外貌 与 RPC客户端刷新容貌.
									InPlayerCharacter->UpdateKneadingBoby(InPlayerState->GetCA());
									InPlayerCharacter->CallUpdateKneadingBobyOnClient(InPlayerState->GetCA());
								}
								return MethodUnit::EServerCallType::PROGRESS_COMPLETE;// 所有步骤完成后就断开不再遍历.
							}
						}

						return MethodUnit::EServerCallType::INPROGRESS;// 没找到合适的人就继续遍历.
					});
				//
			}
			break;
		}

		/** 接收来自服务器的 获取属性集(人物的各项属性,血蓝技能连招肢体动作)响应. */
		case SP_GetCharacterDataResponses:
		{
			int32 UserID = INDEX_NONE;
			FString CharacterJsonString;
			SIMPLE_PROTOCOLS_RECEIVE(SP_GetCharacterDataResponses, UserID, CharacterJsonString);

			if (UserID != INDEX_NONE) {
				MethodUnit::ServerCallAllPlayerController<AMMOARPGPlayerController>(GetWorld(), [&](AMMOARPGPlayerController* InController)->MethodUnit::EServerCallType {
					if (AMMOARPGPlayerCharacter* InPlayerCharacter = InController->GetPawn<AMMOARPGPlayerCharacter>()) {
						if (InPlayerCharacter->GetUserID() == UserID) {
							FMMOARPGCharacterAttribute CharacterAttribute;// 构造1个人物属性集
							
							// Lambda--将一组字符串从_下划线拼接形式转化为一组FName.
							auto ToGamePlayTags = [&](TArray<FName>& InNames) {
								TArray<FName> OutNames;
								AnalysisArrayNameToGamePlayTags(InNames, OutNames);
								InNames = OutNames;
							};
							
							// 从JSON源里解析出字符串.
							NetDataAnalysis::StringToMMOARPGCharacterAttribute(CharacterJsonString, CharacterAttribute);

							// 技能信息的原位转换(从从_下划线拼接形式转化为一组FName.)
							/// 各种形式技能组的数据来源都是来自于从服务器JSON里解析出来的.
							ToGamePlayTags(CharacterAttribute.ComboAttack);
							ToGamePlayTags(CharacterAttribute.Skill);
							ToGamePlayTags(CharacterAttribute.Limbs);

							// 给人更新属性集
							InPlayerCharacter->UpdateCharacterAttribute(CharacterAttribute);

							// 给人更新技能(按形式来源,一共三种,分别是combo连招, Skill能力, limb肢体行为)
							InPlayerCharacter->RegisterGameplayAbility(CharacterAttribute.ComboAttack, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK);
							InPlayerCharacter->RegisterGameplayAbility(CharacterAttribute.Skill, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
							InPlayerCharacter->RegisterGameplayAbility(CharacterAttribute.Limbs, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_LIMBS);

							// 广播行为.
							// 还需要注册一下 连招黑盒
							InPlayerCharacter->RegisterComboAttackMulticast(CharacterAttribute.ComboAttack);

							return MethodUnit::EServerCallType::PROGRESS_COMPLETE;
						}
					}
					return MethodUnit::EServerCallType::INPROGRESS;
					});
			}

			break;
		}
	}
}

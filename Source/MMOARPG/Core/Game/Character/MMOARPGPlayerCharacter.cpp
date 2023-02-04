// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOARPGPlayerCharacter.h"
#include "../MMOARPGPlayerState.h"
#include "../../Common/MMOARPGGameInstance.h"
#include "../MMOARPGGameMode.h"
#include "ThreadManage.h"
#include "../../../MMOARPGMacroType.h"

void AMMOARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetRelativeLocation());

	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy || GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {
	#if !UE_MMOARPG_DEBUG_DS
		// 仅当未开启调试才走 刷新人物样貌.
		if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
			FlushKneadingRequest();
		}
	#endif

		/// (该代码是为了做切换准备的 更新捏脸数据) 反复确保只要生成人物就实时拿到PS-CA. 
		if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>()) {
			UpdateKneadingBoby(InPlayerState->GetCA());// 第二次第三次进来之后要求实时刷新到最新PS里的CA.
		}

		GThread::Get()->GetCoroutines().BindLambda(1.5f, [&]() ->void {
		
			});
	}
}

void AMMOARPGPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
// #if UE_MMOARPG_DEBUG_DS
// 	GameCount = 0;
// #endif
// 
	Super::EndPlay(EndPlayReason);
}

void AMMOARPGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 不走服务器,只让主机玩家和模拟玩家走
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy || GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {
		// 获取到玩家状态.
		if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>()) {
			// 核验人物当前运行阶段是否匹配"捏脸阶段".
			if (InPlayerState->GetGameStatusType() == EMMOARPGGameStatusType::MMOARPG_PINCH_REQUEST_STATUS) {
				// PS里的CA有意义,证明服务器已把外貌数据同步过来
				if (InPlayerState->GetCA().IsVaild()) {
					// 刷新人物外貌.
					UpdateKneadingBoby(InPlayerState->GetCA());
					// 设置人物的下一个运行阶段
					InPlayerState->SetGameStatusType(EMMOARPGGameStatusType::MMOARPG_DATA_ACQUISITION_PHASE);
				}
			}
			// 接上文,进入第二阶段, 是否匹配"数据注册阶段".
			else if (InPlayerState->GetGameStatusType() == EMMOARPGGameStatusType::MMOARPG_DATA_ACQUISITION_PHASE) {
				// 两种客户端都 RPC服务器去执行人物数据注册.
				GetCharacterDataRequests();
				// 设置人物的下一个运行阶段
				InPlayerState->SetGameStatusType(EMMOARPGGameStatusType::MMOARPG_IN_GAME);
			}
			// 接上文,又进入第三阶段
			else if (InPlayerState->GetGameStatusType() == EMMOARPGGameStatusType::MMOARPG_IN_GAME) {
				TickGame(DeltaTime);
			}
		}
	}
}

void AMMOARPGPlayerCharacter::UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);
	SetHeadSize(InCA.HeadSize);
	SetChestSize(InCA.ChestSize);
	//需要Mesh也要和我们的骨骼一致变化
	SetMeshPostion(GetMesh());
}

void AMMOARPGPlayerCharacter::UpdateKneadingBoby()
{

}

/** RPC在DS-GM, 发送刷新容貌的请求. */
void AMMOARPGPlayerCharacter::CallServerUpdateKneading_Implementation(int32 InUserID)
{
	// 这时候已真正意义上位于DS服务器上; 并让DS RPC

	if (AMMOARPGGameMode* InGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>()) {// 拿取DS上真正存在的GM.

		AMMOARPGCharacterBase::UserID = InUserID;
		InGameMode->LoginCharacterUpdateKneadingRequest(InUserID);// 让登录到DS的玩家发送刷新容貌身材请求.
	}
}

// 在人物运行的第三阶段"In-Game"阶段下执行的逻辑.
void AMMOARPGPlayerCharacter::TickGame(float DeltaTime)
{

}

/** RPC在客户端, 执行刷新容貌. */
// void AMMOARPGPlayerCharacter::CallUpdateKneadingBobyOnClient_Implementation(const FMMOARPGCharacterAppearance& InCA)
// {
// 	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy || GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {
// 		UpdateKneadingBoby(InCA);// 客户端上刷新人物样貌.
// 
// 		if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>()) {
// 			InPlayerState->GetCA() = InCA;// 在PS内存一份.
// 		}
// 
// 		// 只在本机玩家客户端 上 刷新PS里的最新CA.
// 		if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
// 			/** 刷新容貌的时候, 这是第一次向CS请求人物属性集 */
// 			GetCharacterDataRequests();
// 		}
// 	}
// }

void AMMOARPGPlayerCharacter::FlushKneadingRequest()
{
	if (UMMOARPGGameInstance* InGameInstance = GetWorld()->GetGameInstance<UMMOARPGGameInstance>()) {
	#if UE_MMOARPG_DEBUG_DS
		// RPC在DS - GM, 发送刷新容貌的请求.

		// 让指定的用户号存档切换

		if (GameCount <= 3) {
			CallServerUpdateKneading(23);/** 这里的数字必须是用户号user_id 如不清楚可查询数据库机器上的wp_usermeta表 */
			GameCount += 3;
		}
		else if (GameCount == 3) {
			CallServerUpdateKneading(23);
			GameCount++;
		}
		else if (GameCount >= 4) {
			CallServerUpdateKneading(24);
			GameCount = 0;
		}

	#else
		// RPC在DS - GM, 发送刷新容貌的请求.
		CallServerUpdateKneading(InGameInstance->GetUserData().ID);
	#endif
		}
	}


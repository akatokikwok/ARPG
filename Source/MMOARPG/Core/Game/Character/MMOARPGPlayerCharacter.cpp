// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOARPGPlayerCharacter.h"
#include "../MMOARPGPlayerState.h"
#include "../../Common/MMOARPGGameInstance.h"
#include "../MMOARPGGameMode.h"
#include "ThreadManage.h"
#include "../../../MMOARPGMacroType.h"

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

void AMMOARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetRelativeLocation());

	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {// 满足是本机玩家.

		GThread::Get()->GetCoroutines().BindLambda(1.5f, [&]() ->void {
		
			/// 反复确保只要生成人物就实时拿到PS-CA.
			if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>()) {
				UpdateKneadingBoby(InPlayerState->GetCA());// 第二次第三次进来之后要求实时刷新到最新PS里的CA.
			}
		#if !UE_MMOARPG_DEBUG_DS // 仅当未开启调试才走 刷新人物样貌.
			FlushKneadingRequest();
		#endif
		
		}
		);
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {// 满足是模拟玩家.

	}
}

void AMMOARPGPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GameCount = 0;
	Super::EndPlay(EndPlayReason);
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

/** RPC在客户端, 执行刷新容貌. */
void AMMOARPGPlayerCharacter::CallUpdateKneadingBobyOnClient_Implementation(const FMMOARPGCharacterAppearance& InCA)
{
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy || GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {
		UpdateKneadingBoby(InCA);// 客户端上刷新人物样貌.

		if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>()) {
			InPlayerState->GetCA() = InCA;// 在PS内存一份.
		}

		// 只在本机玩家客户端 上 刷新PS里的最新CA.
		if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
			/** 刷新容貌的时候, 这是第一次向CS请求人物属性集 */
			GetCharacterDataRequests();
		}
	}
}

void AMMOARPGPlayerCharacter::FlushKneadingRequest()
{
	if (UMMOARPGGameInstance* InGameInstance = GetWorld()->GetGameInstance<UMMOARPGGameInstance>()) {
#if UE_MMOARPG_DEBUG_DS
		// RPC在DS - GM, 发送刷新容貌的请求.

		// 让指定的用户号存档切换
		
		if (GameCount == 0 || GameCount == 1) {
			CallServerUpdateKneading(1);
			GameCount += 3;
		}
		else if (GameCount == 2) {
			CallServerUpdateKneading(1);
			++GameCount;
		}
		else if (GameCount == 3 || GameCount == 4) {
			CallServerUpdateKneading(3);
			GameCount = 0;
		}

#else
		// RPC在DS - GM, 发送刷新容貌的请求.
		CallServerUpdateKneading(InGameInstance->GetUserData().ID);	
#endif
	}
}


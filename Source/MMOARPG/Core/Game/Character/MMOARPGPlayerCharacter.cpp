// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOARPGPlayerCharacter.h"
#include "../MMOARPGPlayerState.h"
#include "../../Common/MMOARPGGameInstance.h"
#include "../MMOARPGGameMode.h"

void AMMOARPGPlayerCharacter::UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);

	//需要Mesh也要和我们的骨骼一致变化
	SetMeshPostion(GetMesh());
}

void AMMOARPGPlayerCharacter::UpdateKneadingBoby()
{
	
}

void AMMOARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetComponentLocation());

	// 
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {// 满足是服务器.
		if (UMMOARPGGameInstance* InGins = GetWorld()->GetGameInstance<UMMOARPGGameInstance>()) {
			// 开始游戏的时候就主动 RPC在DS-GM, 发送刷新容貌的请求.
			CallServerUpdateKneading(InGins->GetUserData().ID);// 拿取客户端中GINS里存储的用户数据ID, 并呼叫DS去刷新登录人物样貌.
		}
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {// 满足是模拟玩家.

	}
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
	UpdateKneadingBoby(InCA);// 客户端上刷新人物样貌.
	
}


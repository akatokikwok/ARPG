// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGPlayerController.h"
#include "Character/MMOARPGCharacter.h"
#include "Character/MMOARPGPlayerCharacter.h"
#include "MMOARPGGameState.h"
#include "MMOARPGPlayerState.h"
#include "../../MMOARPGGameMethod.h"

AMMOARPGPlayerController::AMMOARPGPlayerController()
{
	bShowMouseCursor = true;
}

void AMMOARPGPlayerController::ReplaceCharacter_Implementation(int32 InCharacterID)
{
	// 异常检测1.
	if (!GetPawn()) {
		return;
	}

	// 异常检测2.
	if (AMMOARPGCharacterBase* MMOARPGBase = GetPawn<AMMOARPGCharacterBase>())// 先转成Base
	{
		if (MMOARPGBase->GetID() == InCharacterID) {// 若是本人物匹配了就无需替换人物.
			return;
		}
	}

	// 替换人物的主逻辑. 数据存在GameState的DataTable里.
	if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
		if (FCharacterStyleTable* InStyleTable_row = InGameState->GetCharacterStyleTable(InCharacterID)) {// 把DT里这一行读出来

			if (AMMOARPGCharacter* InNewCharacter = GetWorld()->SpawnActor<AMMOARPGCharacter>(// 生成1个新角色.
				InStyleTable_row->MMOARPGCharacterClass,
				GetPawn()->GetActorLocation(),
				GetPawn()->GetActorRotation())) {

				if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>()) {
					//判断是主要玩家角色, 而非其他类型的野怪或者随从.
					if (AMMOARPGPlayerCharacter* InPlayerCharacter = Cast<AMMOARPGPlayerCharacter>(InNewCharacter)) {
						InPlayerCharacter->UpdateKneadingBoby(InPlayerState->GetCA());// 刷新容貌
// 						InPlayerCharacter->CallUpdateKneadingBobyOnClient(InPlayerState->GetCA());// RPC客户端刷新容貌.
					}

					APawn* InPawn = GetPawn();// 在改控制之前, 先拿取旧人物.
					OnPossess(InNewCharacter);// 改为控制替换用的角色.
					InPawn->Destroy(true);// 设为TRUE表示强制网络删除.
				}

			}
		}
	}
}

// RPC在服务器, 写入目标人物或者小怪.
void AMMOARPGPlayerController::ResetTargetOnServer_Implementation(AMMOARPGCharacterBase* InNewTarget)
{
	Target = InNewTarget;
}

void AMMOARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AMMOARPGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// 检索半径范围内最近的敌对目标.
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy || 
		GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {
		if (GetPawn() != nullptr) {

			float MaxNewRange = 820.f;
			// 找寻520米内最近的敌对目标并在服务器上将其写入
			if (!Target.IsValid()) {
				TArray<ECharacterType> IgnoreTypes;
				Target = MMOARPGGameMethod::FindTarget(Cast<AMMOARPGCharacterBase>(GetPawn()), IgnoreTypes, MaxNewRange);
				if (Target.IsValid()) {
					/* 仅在主机端执行写入目标*/
					if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
						ResetTargetOnServer(Target.Get());
					}
				}
			}
			// 意外情况,敌人太远,或者敌人死亡,就清空敌对目标并在服务器上也写入一次.
			else {
				float Distance = FVector::Dist(Target->GetActorLocation(), GetPawn()->GetActorLocation());
				if (Distance > MaxNewRange || Target->IsDie()) {
					Target = NULL;
					/* 仅在主机端执行写入目标*/
					if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
						ResetTargetOnServer(nullptr);
					}
				}
			}
		}
	}
}

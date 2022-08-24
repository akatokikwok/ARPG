// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGPlayerState.h"
#include "Net/UnrealNetwork.h"

AMMOARPGPlayerState::AMMOARPGPlayerState()
{
	// 初始化游戏阶段为"捏脸注册阶段".
	GameStatusType = EMMOARPGGameStatusType::MMOARPG_PINCH_REQUEST_STATUS;
}

void AMMOARPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 指明让这一份外貌发生变化时执行同步.
	DOREPLIFETIME(AMMOARPGPlayerState, CA);
}

void AMMOARPGPlayerState::SetGameStatusType(EMMOARPGGameStatusType InNewType)
{
	GameStatusType = InNewType;
}

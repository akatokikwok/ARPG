// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MMOARPGType.h"
#include "../../MMOARPGGameType.h"
#include "MMOARPGPlayerState.generated.h"

/**
 * MMOARPG项目的玩家状态.
 * 持有一些数据,例如外貌CA.
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMMOARPGPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	// 拿取CA存档.
	FMMOARPGCharacterAppearance& GetCA() { return CA; }

	// 获取玩家处于的游戏阶段.
	EMMOARPGGameStatusType GetGameStatusType()const { return GameStatusType; }

	// 设定玩家处于的游戏阶段.
	void SetGameStatusType(EMMOARPGGameStatusType InNewType);

private:
	// 相貌数据允许被同步.
	UPROPERTY(Replicated)
		FMMOARPGCharacterAppearance CA;

	// 每个角色都会拥有的"游戏阶段".
	EMMOARPGGameStatusType GameStatusType;
};

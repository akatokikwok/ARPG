// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Common/Core/GamePlay/PlayerControllerBase.h"
#include "MMOARPGPlayerController.generated.h"

// 委托:更新技能page或者是更新技能slot
DECLARE_DELEGATE_OneParam(FUpdateSkillDelegate, const TArray<FName>&)

// 委托:通知客户端更新CD; 需要技能名字和冷却时长
DECLARE_DELEGATE_TwoParams(FUpdateSkillCooldownDelegate, const FName&, float)

class AMMOARPGCharacter;
/**
 *
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerController : public APlayerControllerBase
{
	GENERATED_BODY()
public:
	FUpdateSkillDelegate UpdateSkillTableDelegate;// 委托:更新技能表
	FUpdateSkillDelegate UpdateSkillSlotDelegate; // 委托:更新技能节点
	FUpdateSkillCooldownDelegate UpdateSkillCooldownDelegate;// 委托:通知客户端更新CD

public:
	AMMOARPGPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnSureButtonClicked(uint8 InProtocol) override;

public:
	// 运行在服务器.
	UFUNCTION(server, reliable)
		void ReplaceCharacter(int32 InCharacterID);

	// RPC在服务器, 写入目标人物或者小怪.
	UFUNCTION(server, reliable)
		void ResetTargetOnServer(AMMOARPGCharacterBase* InNewTarget);

	// 拿取目标人物或者小怪.
	AMMOARPGCharacterBase* GetTarget() { return Target.Get(); }

	// 通知客户端更新CD; 需要技能名字和冷却时长
	UFUNCTION(Client, Reliable)
		void CallUpdateCooldownOnClient(const FName& InTagName, float InCooldownValue);

protected:
	// 服务端执行人物重生
	UFUNCTION(Server, Reliable)
	void ResurrectionOnServer();

protected:
	// 敌对人物或者小怪,人物基类弱指针;
	TWeakObjectPtr<AMMOARPGCharacterBase> Target;
};
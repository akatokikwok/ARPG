// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Common/Core/GamePlay/PlayerControllerBase.h"
#include "MMOARPGPlayerController.generated.h"

class AMMOARPGCharacter;
/**
 *
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerController : public APlayerControllerBase
{
	GENERATED_BODY()
public:
	AMMOARPGPlayerController();
	
	// 运行在服务器.
	UFUNCTION(server, reliable)
		void ReplaceCharacter(int32 InCharacterID);
	// RPC在服务器, 写入目标人物或者小怪.
	UFUNCTION(server, reliable)
		void ResetTargetOnServer(AMMOARPGCharacterBase* InNewTarget);
	
	// 拿取目标人物或者小怪.
	AMMOARPGCharacterBase* GetTarget() { return Target.Get(); }
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// 敌对人物或者小怪,人物基类弱指针;
	TWeakObjectPtr<AMMOARPGCharacterBase> Target;

};

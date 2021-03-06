// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMOARPGCharacter.h"
#include "../../Common/Interface/KneadingInterface.h"
#include "MMOARPGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerCharacter : public AMMOARPGCharacter, public IKneadingInterface
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// RPC,在DS端的GM 执行发送刷新登录人物样貌请求.
	UFUNCTION(server, Reliable)
		void CallServerUpdateKneading(int32 InUserID);
public:
	// 刷新人物的外貌身材.可用于DS-GM上/客户端亦可.
	virtual	void UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA) override;
	// 刷新人物的外貌身材.可用于DS-GM上/客户端亦可.
	virtual	void UpdateKneadingBoby() override;

	// 广播 执行刷新登录人物样貌.
	UFUNCTION(NetMulticast, Reliable)
		void CallUpdateKneadingBobyOnClient(const FMMOARPGCharacterAppearance& InCA);

	// 绑定用的回调, 刷新操作人物相貌请求.
	UFUNCTION()
		void FlushKneadingRequest();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Tickable.h"
#include "../Plugins/SimpleNetChannel/Source/SimpleNetChannel/Public/SimpleNetManage.h"
#include "MMOARPGType.h"
#include "MMOARPGGameInstance.generated.h"

/**
 *
 */
UCLASS()
class MMOARPG_API UMMOARPGGameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// override from Gameinstance.
	virtual void Init();
	// override from FTickableGameObject.
	virtual void Tick(float DeltaTime);
	// 重载 获取状态ID.
	virtual TStatId GetStatId() const;
	// Override from GameInstance. 销毁客户端.
	virtual void Shutdown();

public:
	// 创建客户端.
	void CreateClient();
	// 链接至服务器.
	void LinkServer();
	//

	// API. 拿取客户端亦或是服务器.
	FSimpleNetManage* GetClient();

	// 拿取用户数据结构体.
	FMMOARPGUserData& GetUserData();

private:
	FSimpleNetManage* Client;
	FMMOARPGUserData UserData;// 每个gameinstance都有一份用户数据.
};

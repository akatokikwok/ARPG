// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Tickable.h"
#include "../Plugins/SimpleNetChannel/Source/SimpleNetChannel/Public/SimpleNetManage.h"
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
	// ���� ��ȡ״̬ID.
	virtual TStatId GetStatId() const;
	// Override from GameInstance. ���ٿͻ���.
	virtual void Shutdown();

public:
	// �����ͻ���.
	void CreateClient();
	// ���ӷ�����.
	void LinkServer();
	//

	// API.
	FSimpleNetManage* GetClient();

private:
	FSimpleNetManage* Client;
};

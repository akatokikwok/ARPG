// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.h"
#include "UI_MainBase.generated.h"

class UUI_Print;

/**
 * 各级UI主要继承的基类.
 */
UCLASS()
class MMOARPG_API UUI_MainBase : public UUI_Base
{
	GENERATED_BODY()

		UPROPERTY(meta = (BindWidget))
		UUI_Print* UI_Print;

public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

public:
	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

protected:
	/**循环创建与绑定. 绑定客户端的接收.
	 * 因为有概率UI先创建好,但是GIns没有创建
	 * 因此循环创建.
	 */
	void BindClientRcv();

	/** 可以由子类覆写的回调, 用以从客户端发送协议给更高一层的网关. */
	UFUNCTION()
		virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

	/** 让UI链接至服务器,需要1个指定的网关. */
	void LinkServer(const FSimpleAddr& InAddr);

	/**  无参数版*/
	void LinkServer();
private:
	FDelegateHandle mRecvDelegate;
};

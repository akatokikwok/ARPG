// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "Channel/SimpleChannel.h"
#include "UI_LoginMain.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_LoginMain : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;// 重载自 UI_Base.

protected:
	/**循环创建与绑定. 绑定客户端的接收.
	 * 因为有概率UI先创建好,但是GIns没有创建
	 * 因此循环创建.
	 */
	void BindClientRcv();

	/**  客户端向服务端握手的时候代理 绑定的回调.*/
	UFUNCTION()
	void Callback_LinkServerInfo(ESimpleNetErrorType InErrorType, const FString& InMsg);

private:
	FDelegateHandle mRecvDelegate;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* UI_LinkWidget;
};

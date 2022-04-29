// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "Channel/SimpleChannel.h"
#include "UI_Login.h"
#include "UI_LoginMain.generated.h"

class UTextBlock;

/**
 * 一切登录功能的主面板.
 * 入口面板.
 */
UCLASS()
class MMOARPG_API UUI_LoginMain : public UUI_Base
{
	GENERATED_BODY()
	friend class UUI_Login;
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** 重载UI_Base重要方法: RecvProtocol */
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

protected:
	/**循环创建与绑定. 绑定客户端的接收.
	 * 因为有概率UI先创建好,但是GIns没有创建
	 * 因此循环创建.
	 */
	void BindClientRcv();

	/**  账户登录的真正入口逻辑.*/
	void SignIn(const FString& InAccount, const FString& InPassword);
	
	/**  注册账户的真正入口逻辑.*/
	void Register();

protected:
	// 客户端向服务端握手的时候代理 绑定的回调.
	UFUNCTION()
	void Callback_LinkServerInfo(ESimpleNetErrorType InErrorType, const FString& InMsg);

	void PrintLog(const FString& InMsg);

private:
	FDelegateHandle mRecvDelegate;
	
	// 监测是否链接上服务器的控件(会显示转圈圈动画).
	UPROPERTY(meta = (BindWidget))
	UUserWidget* UI_LinkWidget;
	// 深蓝色登录界面面板.
	UPROPERTY(meta = (BindWidget))
	UUI_Login* UI_Login;
	// 屏幕最下面打印日志.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MsgLog;
};

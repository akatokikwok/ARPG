// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "../Common/UI_Print.h"
#include "UI_HallMain.generated.h"

/**
 * 大厅主界面Widget.
 */
UCLASS()
class MMOARPG_API UUI_HallMain : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 屏幕打印日志.
	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

protected:
	/** 循环创建与绑定. 绑定客户端的接收. */
	void BindClientRcv();
	/** 重载UI_Base重要方法: RecvProtocol */
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

private:
	// 接收代理.
	FDelegateHandle mRecvDelegate;
	// 打印控件.
	UPROPERTY(meta = (BindWidget))
	UUI_Print* UI_Print;
};

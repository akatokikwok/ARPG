#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_MainBase.h"
#include "UI_LoginMain.generated.h"

class UTextBlock;

/**
 * 一切登录功能的主面板.
 * 入口面板.
 */
UCLASS()
class MMOARPG_API UUI_LoginMain : public UUI_MainBase
{
	GENERATED_BODY()
private:
	// 监测是否链接上服务器的控件(会显示转圈圈动画).
	UPROPERTY(meta = (BindWidget))
		UUserWidget* UI_LinkWidget;
	// 深蓝色登录界面面板.
	UPROPERTY(meta = (BindWidget))
		UUI_Login* UI_Login;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	/** 重载MainBase重要方法: RecvProtocol; */
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;
	/** 重载MainBase重要方法: LinkServerInfo; 客户端向服务端握手的时候代理 绑定的回调. */
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg) override;

public:
	/**  账户登录的真正入口逻辑. 为了正确解析,入参不推荐加const修饰.*/
	void SignIn(/*const*/ FString& InAccount, /*const*/ FString& InPassword);
	/**  注册账户的真正入口逻辑.*/
	void Register();

};

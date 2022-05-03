// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_Login.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 登录面板.(携带输入账户密码)
 */
UCLASS()
class MMOARPG_API UUI_Login : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	/**  */
	UFUNCTION()
	void SignIn();
	/**  */
	UFUNCTION()
	void Register();

	/** 加密账户密码到本地.*/
	bool EncryptionToLocal(const FString& InPaths);
	/** 从本地解密密码. */
	bool DecryptionFromLocal(const FString& InPaths);

private:
	// Account账户输入框; BindWidget意为把蓝图内容暴露至C++.
	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* Account;
	// 密码输入框
	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* Password;
	// 登录按钮.
	UPROPERTY(meta = (BindWidget))
		UButton* SignInButton;
	// 注册按钮.
	UPROPERTY(meta = (BindWidget))
		UButton* RegisterButton;
};

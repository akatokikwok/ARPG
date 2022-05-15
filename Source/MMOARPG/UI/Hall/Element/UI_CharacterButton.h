// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "UI_CharacterButton.generated.h"

/**
 * 点击可以完成创建角色的按钮.
 */
UCLASS()
class MMOARPG_API UUI_CharacterButton : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// 用于点击CharacterButton被绑定的方法.
	UFUNCTION()
	void Callback_ClickedCharacter();

private:
	// 暴露来自蓝图的控件子项至Cpp.
	UPROPERTY(meta = (BindWidget))
		UButton* CharacterButton;
	UPROPERTY(meta = (BindWidget))
		UImage* CreateShowIcon;
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* CharacterInfo;
	UPROPERTY(meta = (BindWidget))
		UImage* CharacterIcon;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CharacterName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Date;

};

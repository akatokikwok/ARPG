// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
#include "MMOARPGType.h"
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

	/** 设置指定数据包的+号按钮外观. */
	void InitCharacterButton(const FMMOARPGCharacterAppearance& InCA);
	/** 使高亮 CharacterButton. */
	void SetHighlight(bool bHighlight);

	// 覆写槽序数.
	void SetSlotPosition(const int32 InNewPos);
	/** 拿本类的槽序数. */
	FORCEINLINE int32 GetSlotPosition() { return SlotPosition; }

protected:
	// 用于点击CharacterButton被绑定的方法.
	UFUNCTION()
		void ClickedCharacter_callback();

protected:
	int32 SlotPosition;// 槽序数.在父级面板初始化按钮的时候将此槽序初始化.
	FLinearColor DefaultColor;// CharacterButton 非高亮情况下的默认颜色.

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

	// CharacterButton的高亮颜色,由编辑器设定.
	UPROPERTY(EditDefaultsOnly, Category = UI, meta = (ToolTip = "CharacterButton的高亮颜色,由编辑器设定."))
		FLinearColor HighlightColor;

};

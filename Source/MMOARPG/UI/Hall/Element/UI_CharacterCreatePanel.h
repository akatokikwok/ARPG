// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterButton.h"
#include "UI_CharacterCreatePanel.generated.h"

/**
 * 负责创建角色的面板.
 */
UCLASS()
class MMOARPG_API UUI_CharacterCreatePanel : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void InitCharacterButton(const int32 InNumber);

private:
	// 暴露来自蓝图的控件子项至Cpp.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PanelName;
	// 滑动框; 暴露来自蓝图的控件子项至Cpp.
	UPROPERTY(meta = (BindWidget))
		UScrollBox* List;

	// 被动态添加进滑动列表的 按钮; 借助EditDefaultsOnly元元素说明符来将其反射至蓝图.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUI_CharacterButton> UI_CharacterButtonClass;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterButton.h"
#include "UI_KneadFace.h"
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

	// 负责 当点击加号后创建 捏脸信息的逻辑.
	void CreateKneadFace();
	// 创建 加号UI.
	void CreateCharacterButtons();

protected:
	// 初始化指定数量的 加号UI 在滑动框里.
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

	// 捏脸的UI 控件.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUI_KneadFace> UI_KneadFaceClass;

};

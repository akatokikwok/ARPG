// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterButton.h"
#include "UI_KneadFace.h"
#include "MMOARPGType.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
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
	/** 创建所有加号.(角色数据来源是PS) */
	void CreateCharacterButtons();

public:
	/** 初始化所有加号.(默认设定是4个) */
	void InitCharacterButtons(const FCharacterAppearances& InCAs);
	/** 仅负责生成玩家人物. */
	void SpawnCharacter();
	/** 生成指定槽号的玩家形象 */
	void SpawnCharacter(const int32 InSlotIndex);
	/** 生成 特定CA存档数据下的 玩家形人物 */
	void SpawnCharacter(const FMMOARPGCharacterAppearance* InACData);

	/** 覆写当前槽号(记录或保留当前槽号) */
	void SetCurrentSlotPosition(const int32 InNewPos);

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

	// 待创建人物.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<ACharacterStage> CharacterStageClass;
	// 待创建人物的生成位置.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		FVector SpawnPoint;

protected:
	int32 SlotPosition;// 保存当前被选中的人物槽号序数.
};

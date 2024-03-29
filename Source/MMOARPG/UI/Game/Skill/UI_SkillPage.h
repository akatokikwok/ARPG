﻿#pragma once

#include "CoreMinimal.h"
#include "UI_MovePage.h"
#include "UI_SkillPage.generated.h"

class UUniformGridPanel;
class UUI_SkillSlot;
class UButton;

/**
 * 技能页(技能表)
 * 继承自 允许拖拽自身的控件UUI_MovePage
 */
UCLASS()
class MMOARPG_API UUI_SkillPage : public UUI_MovePage
{
	GENERATED_BODY()
public:
	//
	UPROPERTY(meta = (BindWidget))
		UButton* CloseButton;
	//
	UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* SlotArrayInventory;
	//
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUI_SkillSlot> SkillSlotClass;

public:
	virtual void NativeConstruct() override;

public:
	// 以一组技能标签来构建外观布局
	void LayoutSlot(const TArray<FName>& InKeys);
	
	//
	UFUNCTION()
		void OnClose();

	//
	UFUNCTION()
		void OnClickedWidget();

protected:
	// 回调, 用于绑定 委托-"更新技能表".
	UFUNCTION()
		void UpdateSkillTable(const TArray<FName>& InSkillTags);

	// 以某个Lambda来处理技能页里的技能槽
	void CallSKillSlot(TFunction<bool(UUI_SkillSlot*)> InFunction);

public:
	// 设置横框内的某个技能槽高亮
	UFUNCTION()
		void SetBorderHeight(EMMOARPGSkillType InSkillType);

	// 处理高亮的接口, 复位高亮
	UFUNCTION()
		void ResetBorderHeight();
};
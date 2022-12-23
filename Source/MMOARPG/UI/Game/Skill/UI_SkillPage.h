﻿#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_SkillPage.generated.h"

class UUniformGridPanel;
class UUI_SkillSlot;

/**
 * 技能页(技能表)
 */
UCLASS()
class MMOARPG_API UUI_SkillPage : public UUI_Base
{
	GENERATED_BODY()
public:
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

	// 回调, 用于绑定 委托-"更新技能表".
	UFUNCTION()
		void UpdateSkillTable(const TArray<FName>& InSkillTags);
};
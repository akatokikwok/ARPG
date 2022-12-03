#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_SkillPage.generated.h"

class UUniformGridPanel;
class UUI_SkillSlot;

/**
 * 技能页
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
	// 一组技能标签
	void LayoutSlot(const TArray<FName>& InKeys);
	
	//
	UFUNCTION()
	void UpdateSkillTable(const TArray<FName>& InSkillTags);
};
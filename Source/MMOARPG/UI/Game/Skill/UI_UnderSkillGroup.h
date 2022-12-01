#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_UnderSkillGroup.generated.h"

class UHorizontalBox;
class UUI_SkillSlot;

/**
 * HUD上一派横格子的技能组
 */
UCLASS()
class MMOARPG_API UUI_UnderSkillGroup : public UUI_Base
{
	GENERATED_BODY()
public:
	//
	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* SlotArray;
	//
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_SkillSlot> SkillSlotClass;

public:
	virtual void NativeConstruct() override;

public:

};
#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_UnderSkillGroup.generated.h"

class UHorizontalBox;
class UUI_SkillSlot;

/**
 * HUD内 下侧的技能横框
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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	// 初始化
	void LayoutSlot(const TArray<FName>& InSkillTags);
	
	// 是否人为屏蔽了技能输入
	bool IsShieldSkill() { return *bShieldSkill; }

protected:
	// 回调, 用于绑定 委托-"更新技能节点".
	UFUNCTION()
		void UpdateSkillSlots(const TArray<FName>& InSkillTags);

	// 回调, 用于绑定技能CD
	UFUNCTION()
		void UpdateSkillCD(const FName& InTagName, float InCDValue);

	// 使用传入的lambda专门处理技能横框的特定slot
	void CallSKillSlot(TFunction<bool(UUI_SkillSlot*)> InFunction);

	// 屏蔽技能输入
	void ShieldSkillInput(float InTime);

	//
	void ShieldSkillInput(bool bShield);

private:
	// 刷新切换BOOL状态的结构
	FResetBool bShieldSkill;
};
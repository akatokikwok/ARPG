#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UI_SkillSlot.generated.h"

class UImage;
class UButton;

/**
 * 技能UI插槽
 */
UCLASS()
class MMOARPG_API UUI_SkillSlot : public UUI_Base
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		UImage* SlotIcon;

	UPROPERTY(meta = (BindWidget))
		UButton* SkillButton;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// 敲击后的回调
	UFUNCTION()
		void OnClickedWidget();
};
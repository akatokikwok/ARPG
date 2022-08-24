#pragma once

#include "CoreMinimal.h"
#include "UI_HealthState.h"
#include "UI_CharacterHealthWidget.generated.h"

class UTextBlock;

/**
 * 负责显示玩家悬浮血条的UI
 * 派生自简易UI血条类: UUI_HealthState
 */
UCLASS()
class MMOARPG_API UUI_CharacterHealthWidget : public UUI_HealthState
{
	GENERATED_BODY()
public:
	// 设定血条持有者的姓名
	void SetCharacterName(const FText& InCharacterName);
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CharacterName;

};

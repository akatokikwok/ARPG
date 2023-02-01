#pragma once
#include "CoreMinimal.h"
#include "../Common/Core/GamePlay/HUDBase.h"
#include "MMOARPGHUD.generated.h"
class UUI_GameMain;
class UUI_ComboCount;

/**
 * 游戏主HUD.
 */
UCLASS()
class MMOARPG_API AMMOARPGHUD : public AHUDBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		TSubclassOf<UUI_GameMain> GameMainClass;

	UPROPERTY()
		TSubclassOf<UUI_ComboCount> ComboCountClass;// 连打计数蓝图模板

public:
	AMMOARPGHUD();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	// 连打计数UI执行效果
	void PlayComboCount();

private:
	UUI_GameMain* GameMain;
};

#pragma once
#include "CoreMinimal.h"
#include "../Common/Core/GamePlay/HUDBase.h"
#include "MMOARPGHUD.generated.h"
class UUI_GameMain;
class UUI_GamePrintf;

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

public:
	AMMOARPGHUD();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

private:
	UUI_GameMain* GameMain;
};

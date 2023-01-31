#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MMOARPGHUD.generated.h"
class UUI_GameMain;
class UUI_GamePrintf;

/**
 * 游戏主HUD.
 */
UCLASS()
class MMOARPG_API AMMOARPGHUD : public AHUD
{
	GENERATED_BODY()
private:
	UPROPERTY()
		TSubclassOf<UUI_GameMain> GameMainClass;

	// GamePrintf模板
	UPROPERTY()
		TSubclassOf<UUI_GamePrintf> GamePrintfClass;

public:
	AMMOARPGHUD();
	virtual void BeginPlay() override;

public:
	// 一组接口,用于GamePrint各类警示效果
	virtual void Print(const FText& InContent, const FString& InColor);
	virtual void LogPrint(const FText& InContent);
	virtual void ErrorPrint(const FText& InContent);
	virtual void WarningPrint(const FText& InContent);

private:
	UUI_GameMain* GameMain;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"
class UUI_GameMain;
class UUI_GamePrintf;

/**
 * HUDBase基类
 */
UCLASS()
class MMOARPG_API AHUDBase : public AHUD
{
	GENERATED_BODY()
protected:
	// GamePrintf模板
	UPROPERTY()
		TSubclassOf<UUI_GamePrintf> GamePrintfClass;
public:
	AHUDBase();
	virtual void BeginPlay() override;

	// 一组接口,用于GamePrint各类警示效果
	virtual void Print(const FText& InContent, const FString& InColor);
	virtual void LogPrint(const FText& InContent);
	virtual void ErrorPrint(const FText& InContent);
	virtual void WarningPrint(const FText& InContent);
};
#pragma once

#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "SNDNumericalBalanceDebugSettings.generated.h"

/**
 * 关于调试表的settings全局
 */
UCLASS(config = SNDNumericalBalanceDebugSettings)
class USNDNumericalBalanceDebugSettings : public UObject
{
	GENERATED_BODY()

public:
	USNDNumericalBalanceDebugSettings();

	//
	UPROPERTY(EditAnywhere)
		bool bIterationLevel;

	// 各条玩家对玩家的行为日志信息
	UPROPERTY(EditAnywhere, Category = "NumericalDeductionNumericalBalanceDebug", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		TArray<FDebugCharactersInfo> DebugCharactersInfo;// 各条玩家对玩家的行为日志信息
};
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

	// 勾选时会启用打印策略
	UPROPERTY(EditAnywhere)
		bool bIterationLevel;// 勾选时会启用打印策略

	// 打印策略, 当且仅当勾选bIterationLevel才启用策略效果; N指向哪个玩家,哪个玩家等级将失效
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIterationLevel"))
		EIterativeDebugPrintMethod IterativeMethod;// 打印策略, 当且仅当勾选bIterationLevel才启用策略效果;  N指向哪个玩家,哪个玩家等级将失效

	// 各条玩家对玩家的行为日志信息
	UPROPERTY(EditAnywhere, Category = "NumericalDeductionNumericalBalanceDebug", meta = (ToolTip = "This content is version base, which is used for automatic iteration."))
		TArray<FDebugCharactersInfo> DebugCharactersInfo;// 各条玩家对玩家的行为日志信息
};
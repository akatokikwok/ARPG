// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecutionCalculation_PlayMontage.generated.h"

/**
 * 这张GEEC负责处理 播放蒙太奇之后的衔接前摇或者后摇
 */
UCLASS()
class MMOARPG_API UExecutionCalculation_PlayMontage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecutionCalculation_PlayMontage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:
	/** 是否是衔接动画, 譬如那些有施法前摇或者后摇的情况 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayMontage)
		bool bCohesiveAnimation;

	// 非衔接情况下的指名蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayMontage, meta = (EditCondition = "!bCohesiveAnimation"))
		UAnimMontage* MontageToPlay;

	/** 预设进度(第多少秒, 超过此进度后剩余部分的动画序列将不被播放), 用来和已播放蒙太奇进度作比较; Used to indicate if this execution uses Passed In Tags */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayMontage)
		float PlayAnimPos;

	// 由蓝图配置的 蒙太奇播放速率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayMontage)
		float PlayRate;

	// 由蓝图配置的 是否停用播放
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayMontage)
		bool bStopAllAnimMontage;

	// 蓝图配置的 起始StartSection
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayMontage)
		FName StartSectionName;
};

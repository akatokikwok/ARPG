// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/AnimInstanceComponentBase.h"
#include "../Instance/Core/MMOARPGAnimInstanceBase.h"
#include "../../../../MMOARPGGameType.h"
#include "MMOARPGClimbingAnimInstance.generated.h"

/**
 * 攀爬系统动画实例.
 * 派生自UAnimInstanceComponentBase.
 */
UCLASS()
class MMOARPG_API UMMOARPGClimbingAnimInstance : public UAnimInstanceComponentBase
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;
private:
	// 爬跳的具体逻辑.
	void ClimpJump();
	// 解算出合适的攀岩跳枚举.
	EClimbingJumpState CalculationClimbingJumpState();
/// //////////////////////////////////////////////////////////////////////////
public:
	// 攀爬状态枚举.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EClimbingState ClimbingState;
	// 是否跳爬吸附.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJumpToClimbing;

	// 是否跳跃爬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJump;
};

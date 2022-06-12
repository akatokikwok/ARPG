// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Instance/Core/MMOARPGAnimInstanceBase.h"
#include "AnimInstanceComponentBase.generated.h"

/**
 * 与各运动系统交互的动画势力类, 派生自UMMOARPGAnimInstanceBase.
 */
UCLASS()
class MMOARPG_API UAnimInstanceComponentBase : public UMMOARPGAnimInstanceBase
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;

	// 映射人在某姿态下的轴向速度至(-1, 1).
	void ResetAxisSpeed(float InNewSpeed);

/// //////////////////////////////////////////////////////////////////////////
public:
	// Blendspace 字段; 控制任意行为或动作的速度 带3个朝向.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector AxisSpeed;

	// Blendspace 字段; 控制某种运动姿态中人在yaw, pitch朝向.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector2D RotationRate;

	// 是否启用急速 某运动动作.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bFast;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Instance/Core/MMOARPGAnimInstanceBase.h"
#include "AnimInstanceComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UAnimInstanceComponentBase : public UMMOARPGAnimInstanceBase
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;

/// //////////////////////////////////////////////////////////////////////////
public:
	// Blendspace 字段; 控制任意行为或动作的速度 带3个朝向.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector AxisSpeed;

	// Blendspace 字段; 控制飞行姿态中人在yaw, pitch朝向.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector2D RotationRate;

};

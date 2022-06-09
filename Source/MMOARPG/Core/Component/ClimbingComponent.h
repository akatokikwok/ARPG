// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../../MMOARPGGameType.h"
#include "Core/MotionComponent.h"
#include "ClimbingComponent.generated.h"

class AMMOARPGCharacterBase;
class UCharacterMovementComponent;
class UCapsuleComponent;
class UCameraComponent;

/**
 * 攀爬系统组件.
 */
UCLASS()
class MMOARPG_API UClimbingComponent : public UMotionComponent
{
	GENERATED_BODY()
public:
	/**
	 * 仿照自 UCharacterMovementComponent::PhysCustom.
	 * 暴露至蓝图给蓝图使用.
	 */
	UFUNCTION(BlueprintCallable, Category = "Climbing")
	virtual void PhysClimbong(float deltaTime, int32 Iterations);

public:
	// 接收具体运动方向的键盘输入.
	void ClimbForwardAxis(float InVlaue);

};

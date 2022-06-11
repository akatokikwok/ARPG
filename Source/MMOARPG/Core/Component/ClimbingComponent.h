﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	UClimbingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	/**
	 * 仿照自 UCharacterMovementComponent::PhysCustom.
	 * 暴露至蓝图给蓝图使用.
	 */
	UFUNCTION(BlueprintCallable, Category = "Climbing")
		virtual void PhysClimbong(float deltaTime, int32 Iterations);

	// 接收具体运动方向的键盘输入(前向轴移动).
	void ClimbingForwardAxis(float InValue);
	// 接收具体运动方向的键盘输入(横向轴移动)
	void ClimbingMoveRightAxis(float InValue);
private:
	/** 监测攀岩的具体射线检测逻辑. */
	void TraceClimbingState(float DeltaTime);

public:
	// 攀爬状态枚举.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EClimbingState ClimbingState;
	// 是否跳爬吸附.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJumpToClimbing;
};
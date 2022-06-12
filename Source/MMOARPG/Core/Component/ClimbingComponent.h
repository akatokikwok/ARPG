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
	
	// 重设一套用于爬跳的 按键逻辑.
	void ResetJump();

	// 启用或设置攀岩.
	void Climbing();
	// 释放攀爬.
	void ReleaseClimbing();
	// 清除攀岩状态.
	void ClearClimbingState();
	// 是否爬矮墙.
	bool IsLowClimbing();
private:
	/** 监测攀岩的具体射线检测逻辑. */
	void TraceClimbingState(float DeltaTime);

	// 给一套自定义的攀岩配置.
	void SetClimbingState(EMovementMode InMode, ECharacterActionState InCharacterActionState, bool bOrientRotationToMovement);
public:
	// 攀爬状态枚举.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EClimbingState ClimbingState;
	// 是否跳爬吸附.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJumpToClimbing;

	// 是否跳跃爬.
	FResetBool bJump;
	// 是否翻越墙.
	FResetBool bWallClimbing;
private:
	// 翻越矮墙射线与墙顶的交点.
	FVector ClimbingTracePoint;
	// 判定是矮墙还是高墙的翻墙高度.
	float ClimbingHeight;
};

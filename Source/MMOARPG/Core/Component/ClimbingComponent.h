﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../../MMOARPGGameType.h"
#include "Core/MotionComponent.h"
#include "Kismet\KismetSystemLibrary.h"
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
private:
	friend class AMMOARPGCharacter;

	/** 结构体: 管理攀爬系统输入 */
	struct FClimbingInput
	{
		FClimbingInput()
			: Value(0)
			, Direction(FVector::ZeroVector)
		{}

		float Value;
		FVector Direction;
	};
public:
	UClimbingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

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

	// 还原攀爬系统为爬墙状态.
	void ResetClimbingState();

	// 清除攀岩状态.
	void ClearClimbingState();
	// 是否爬矮墙.
	bool IsLowClimbing();
	// 是否已切换至攀岩-坠落姿态.
	bool IsDropClimbingState();
	// 设定给力系统的给力值.
	void LaunchCharacter(const FVector& LaunchVelocity);

	// 切换为攀岩-坠落状态.
	void DropClimbingState();

	// 拿取攀岩拐弯类型
	EClimbingTurnState GetTurnState() { return TurnState; }

	// 拿取攀岩跳蒙太奇姿势
	EClimbingMontageState GetClimbingMontageState() { return ClimbingMontageState; }

	// RPC至服务器, "根据bRight启用来注册横向或竖向轴的参数"
	UFUNCTION(Server, Reliable)
		void SetInputVector(float InValue, const FVector& InDirection, bool bRight);

	// 仅在服务器上为人注入输入方向和输入值.
	void UpdateMovement(float DeltaTime);
private:
	/** 监测攀岩的具体射线检测逻辑. */
	void TraceClimbingState(float DeltaTime);

	// 给一套自定义的攀岩配置.
	void SetClimbingState(EMovementMode InMode, ECharacterActionState InCharacterActionState, bool bOrientRotationToMovement);

	// 微调优化翻墙后的人物位置.
	void AdjustmentClimbing(bool bStart = true);

	// 调节坠落给的蹬腿反力方向
	void AdjustmentPendingLaunchVelocity(float DeltaTime);

	/** 任意射线检测的封装接口 */
	float Scanning(FHitResult& HitResult, TFunction<void(FVector&, FVector&)> TraceLocation);

	// 解算出合适的攀岩跳枚举.
	EClimbingMontageState CalculationClimbingJumpState();

private:
	// 广播诸客户端 "复位攀岩跳开关并注册给定蒙太奇"
	UFUNCTION(NetMulticast, Reliable)
		void MulticastJump(EClimbingMontageState InClimbingMontageState);
	
	// RPC服务器 "MulticastJump"
	UFUNCTION(Server, Reliable)
		void JumpToServer(EClimbingMontageState InClimbingMontageState);

	// Jump大逻辑
	void Jump();

public:
	// 攀爬状态枚举.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EClimbingState ClimbingState;
	// 人的位置到矮墙顶渐变差值速度.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimAttrubute")
		float ClimbToWallTopVInterlerpSpeed = 27.0f;

	// 是否跳跃爬.
	FResetBool bJump;
	// 是否翻越墙.
	FResetBool bWallClimbing;
	// 坠落时候的给力.
	FVector PendingLaunchVelocity;
private:
	// 翻越矮墙射线与墙顶的交点.
	FVector ClimbingTracePoint;
	// 判定是矮墙还是高墙的翻墙高度.
	float ClimbingHeight;
	// 攀岩墙角拐弯枚举
	EClimbingTurnState TurnState;
	// 是否激活攀岩-墙角拐弯
	FResetBool bTurn;

	// 是否渲染检测射线.
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute", meta = (AllowPrivateAcces = "true"))
		TEnumAsByte<EDrawDebugTrace::Type> mTraceLineType;

	// 是否渲染检测射线(当翻越矮墙时候).
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute", meta = (AllowPrivateAcces = "true"))
		TEnumAsByte<EDrawDebugTrace::Type> mTraceLineTypeWhenStepWall;

public:
	// 是否跳爬吸附.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJumpToClimbing;

	// 是否步行转攀岩
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bWalkToClimbing;

private:
	FClimbingInput RightInput;
	FClimbingInput ForwardInput;

	UPROPERTY(Replicated);
	FVector InputVector;

private:
	EClimbingMontageState ClimbingMontageState;
};

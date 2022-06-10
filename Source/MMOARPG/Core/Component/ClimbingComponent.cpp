// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

const float MaxDistance = 999999.0f;

UClimbingComponent::UClimbingComponent()
	: Super()
	, ClimbingState(EClimbingState::CLIMBING_NONE)
{

}

void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() &&
		CameraComponent.IsValid()) {

		TraceClimbingState(DeltaTime);// 命令其射线检测.
	}
}

/**
 * 仿照自 UCharacterMovementComponent::PhysCustom.
 * 暴露至蓝图给蓝图使用.
 */
void UClimbingComponent::PhysClimbong(float deltaTime, int32 Iterations)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() &&
		CameraComponent.IsValid()) {

		// 预防单帧过小.
		if (deltaTime < UCharacterMovementComponent::MIN_TICK_TIME) {
			return;
		}

		/* 安全刷新移动组件(扫过一段距离或者弧边), (移动距离, 排除意外万向锁,)*/
		CharacterMovementComponent->Velocity = CharacterMovementComponent->GetLastInputVector() * CharacterMovementComponent->MaxCustomMovementSpeed;// 单位按键输入 * 最大自定义移速.
		const FVector Adjusted = CharacterMovementComponent->Velocity * deltaTime;// 帧间隔的移动距离.
		FHitResult Hit(1.0f);
		CharacterMovementComponent->SafeMoveUpdatedComponent(Adjusted, CharacterMovementComponent->UpdatedComponent->GetComponentQuat(), true, Hit);

	}
}

// 接收具体运动方向的键盘输入(前向轴移动).
void UClimbingComponent::ClimbingForwardAxis(float InValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() &&
		CameraComponent.IsValid()) {

		if (AController* Controller = MMOARPGCharacterBase->GetController()) {
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();// 拿鼠标的Rot.
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			// get forward vector
			/// 改成按Z轴取前向向量,来模拟攀岩.
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);// 改成按Z轴取前向向量,来模拟攀岩.
			MMOARPGCharacterBase->AddMovementInput(Direction, InValue);
		}

		// 		if (InValue >= 0.f) {/* >0相当于键盘事件只按下W. <0相当于键盘事件按下S.*/
		// 			
		// 		}
	}
}

// 接收具体运动方向的键盘输入(横向轴移动)
void UClimbingComponent::ClimbingMoveRightAxis(float InValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() &&
		CameraComponent.IsValid()) {

		const FRotator Rotation = MMOARPGCharacterBase->GetActorRotation();// 拿人自身Rot而非Controller Rot.
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);// Y是人物横向轴移动.X是前向轴移动,Z是垂直轴移动.
		MMOARPGCharacterBase->AddMovementInput(Direction, InValue);

	}
}

/** 监测攀岩的具体射线检测逻辑. */
void UClimbingComponent::TraceClimbingState(float DeltaTime)
{
	FVector ForwardDirection = MMOARPGCharacterBase->GetActorForwardVector();
	FVector LocalLocation = MMOARPGCharacterBase->GetActorLocation();

	FHitResult HitChestResult;
	float ChestDistance = MaxDistance;// 胸膛到射线相交点的距离.
	{
		// 射线检测 -胸.
		TArray<AActor*> ActorsToIgnore1;
		FVector StartTraceChestLocation = LocalLocation;
		StartTraceChestLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight() / 4.0f;// 胸位置
		FVector EndTraceChestLocation = StartTraceChestLocation + ForwardDirection * 100.0f;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTraceChestLocation, EndTraceChestLocation,
			ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore1,
			EDrawDebugTrace::ForOneFrame, HitChestResult, true);

		if (HitChestResult.bBlockingHit) {
			ChestDistance = FVector::Distance(StartTraceChestLocation, HitChestResult.Location);
		}
	}

	FHitResult HitHeadResult;
	float HeadDistance = MaxDistance;// 头颅到射线相交点的距离.
	{
		// 射线检测 -头部偏上.
		TArray<AActor*> ActorsToIgnore2;
		FVector StartTraceHeadLocation = LocalLocation;
		StartTraceHeadLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
		FVector EndTraceHeadLocation = StartTraceHeadLocation + ForwardDirection * 100.0f;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTraceHeadLocation, EndTraceHeadLocation,
			ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore2,
			EDrawDebugTrace::ForOneFrame, HitHeadResult, true);

		if (HitHeadResult.bBlockingHit) {
			HeadDistance = FVector::Distance(StartTraceHeadLocation, HitHeadResult.Location);
		}
	}

	if (HitChestResult.bBlockingHit && HitHeadResult.bBlockingHit) {/* 两根都命中认为是攀岩. */
		if (ChestDistance <= 50.f && HeadDistance <= 50.f) {
			if (ClimbingState == EClimbingState::CLIMBING_CLIMBING) {
				
			}
			else {
				ClimbingState = EClimbingState::CLIMBING_CLIMBING;// 切位爬高墙枚举.
				CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Custom);// 切换为custom的攀岩mode.
				CharacterMovementComponent->bOrientRotationToMovement = false;// 禁用随运动旋转.
				MMOARPGCharacterBase->ResetActionState(ECharacterActionState::CLIMB_STATE);// 切位攀岩姿态.
			}
		}
	}
	else if (HitChestResult.bBlockingHit) {/* 只命中胸, 头没命中则视为翻越*/
		ClimbingState = EClimbingState::CLIMBING_WALLCLIMBING;
	}
	else {
		ClimbingState = EClimbingState::CLIMBING_NONE;
	}
}

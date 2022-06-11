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
	, bJumpToClimbing(false)
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
		bJump.Tick(DeltaTime);// Tick 跳爬动作
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

		// find out which way is forward
		const FRotator Rotation = MMOARPGCharacterBase->GetActorRotation();// 使用人物自身转向.
		const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0);/** 希望同时受到yaw和pitch的影响. */
		// get forward vector
		/// 改成按Z轴取前向向量,来模拟攀岩.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);// 改成按Z轴取前向向量,来模拟攀岩.
		MMOARPGCharacterBase->AddMovementInput(Direction, InValue);
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

/** 重设一套用于爬跳的 按键逻辑. */
void UClimbingComponent::ResetJump()
{
	bJump = true;
	bJump = 1.6f;// 刷新此动作延迟计时.
}

/** 监测攀岩的具体射线检测逻辑. */
void UClimbingComponent::TraceClimbingState(float DeltaTime)
{
	FVector ForwardDirection = MMOARPGCharacterBase->GetActorForwardVector();
	FVector UpDirection = MMOARPGCharacterBase->GetActorUpVector();
	FVector LocalLocation = MMOARPGCharacterBase->GetActorLocation();
	FRotator ActorRotation = MMOARPGCharacterBase->GetActorRotation();


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
		StartTraceHeadLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();// 头颅位置.
		FVector EndTraceHeadLocation = StartTraceHeadLocation + ForwardDirection * 100.0f;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTraceHeadLocation, EndTraceHeadLocation,
			ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore2,
			EDrawDebugTrace::ForOneFrame, HitHeadResult, true);

		if (HitHeadResult.bBlockingHit) {
			HeadDistance = FVector::Distance(StartTraceHeadLocation, HitHeadResult.Location);
		}
	}

	FHitResult HitGroundResult;
	float GroundDistance = MaxDistance;// 脚部到射线相交点的距离.
	{
		// 射线检测 - 在攀爬中脚部到地面.
		TArray<AActor*> ActorsToIgnore3;
		FVector StartTraceGroundLocation = LocalLocation;
		StartTraceGroundLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight();// 脚底板位置.
		FVector EndTraceGroundLocation = StartTraceGroundLocation + (-UpDirection) * 40.f;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTraceGroundLocation, EndTraceGroundLocation,
			ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore3,
			EDrawDebugTrace::ForOneFrame, HitGroundResult, true);

		if (HitGroundResult.bBlockingHit) {
			GroundDistance = FVector::Distance(StartTraceGroundLocation, HitGroundResult.Location);
		}
	}


	if (HitChestResult.bBlockingHit && HitHeadResult.bBlockingHit) {/* 两根都命中认为是攀岩. */
		
		if (ChestDistance <= 80.0f && HeadDistance <= 80.f) {// 因为胶囊体半径是42. 
			
			/* 攀爬中落地*/
			if (ClimbingState == EClimbingState::CLIMBING_CLIMBING) {
				
				if (HitGroundResult.bBlockingHit) {
					if (GroundDistance < 1.f) {
						ClimbingState = EClimbingState::CLIMBING_TOGROUND;

						CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
						CharacterMovementComponent->bOrientRotationToMovement = true;
						MMOARPGCharacterBase->ResetActionState(ECharacterActionState::NORMAL_STATE);

						ActorRotation.Pitch = 0.0f;
						MMOARPGCharacterBase->SetActorRotation(ActorRotation);// 需要人物朝向pitch也清除掉,以防止落下的时候人物是歪斜的.
						bJumpToClimbing = false;
					}
				}
			}
			/* 攀岩且不落地. */
			else if (ClimbingState != EClimbingState::CLIMBING_TOGROUND) {
				ClimbingState = EClimbingState::CLIMBING_CLIMBING;// 切位爬高墙枚举.
				CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Custom);// 切换为custom的攀岩mode.
				CharacterMovementComponent->bOrientRotationToMovement = false;// 禁用随运动旋转.
				MMOARPGCharacterBase->ResetActionState(ECharacterActionState::CLIMB_STATE);// 切位攀岩姿态.
				
				ActorRotation.Pitch = 0.0f;
				MMOARPGCharacterBase->SetActorRotation(ActorRotation);// 需要人物朝向pitch也清除掉,以防止落下的时候人物是歪斜的.
				bJumpToClimbing = false;
			}
		}

		/** 离墙面距离大于43就认为用户不再想攀岩. */
		else {
			if (ClimbingState == EClimbingState::CLIMBING_TOGROUND) {
				ClimbingState = EClimbingState::CLIMBING_NONE;
			}
		}

		/** 非常贴近墙面的时候按脚离地面高度 来启用是否切换跳爬 */
		if (HitGroundResult.bBlockingHit == false) {
			if (bJumpToClimbing) {
				bJumpToClimbing = false;// 浮空且已在跳爬才视为关闭跳爬.
			}
			else {
				bJumpToClimbing = true;// 浮空且不在跳爬中才视为启用跳爬.
			}
		}
		else {
			bJumpToClimbing = false;// 落地之后重置跳爬
		}
	}
	else if (HitChestResult.bBlockingHit) {/* 只命中胸, 头没命中则视为翻越*/
		ClimbingState = EClimbingState::CLIMBING_WALLCLIMBING;
	}
	else {

		/** 攀岩中左移或者右移身体有半截对着空气就释放攀岩状态,落地. */
		if (ClimbingState == EClimbingState::CLIMBING_CLIMBING) {
			ClimbingState = EClimbingState::CLIMBING_NONE;
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
			CharacterMovementComponent->bOrientRotationToMovement = true;
			MMOARPGCharacterBase->ResetActionState(ECharacterActionState::NORMAL_STATE);
			
			ActorRotation.Pitch = 0.0f;
			MMOARPGCharacterBase->SetActorRotation(ActorRotation);// 需要人物朝向pitch也清除掉,以防止落下的时候人物是歪斜的.
			bJumpToClimbing = false;
		}
	}

	/** 爬圆柱体. */
	if (HitChestResult.bBlockingHit) {
		
		// 类似于FindLookAt
		FRotator NewRot = FRotationMatrix::MakeFromX(MMOARPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();
		ActorRotation.Yaw = NewRot.Yaw;
		ActorRotation.Pitch = NewRot.Pitch;
		MMOARPGCharacterBase->SetActorRotation(ActorRotation);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThreadManage.h"

const float MaxDistance = 999999.0f;

UClimbingComponent::UClimbingComponent()
	: Super()
	, ClimbingState(EClimbingState::CLIMBING_NONE)
	, bJumpToClimbing(false)
	, ClimbingHeight(0.f)
// 	, PendingLaunchVelocity(FVector(0,0,0))
{

}

void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	bWallClimbing.Fun.BindLambda([&]() {
		AdjustmentClimbing(false);
		});


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
		bWallClimbing.Tick(DeltaTime);// Tick翻墙.

		AdjustmentPendingLaunchVelocity(DeltaTime);// 调节坠落给的蹬腿反力方向
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

		// 依据是否具备ROOTMOTION来挑选合适的速度.
		if (!CharacterMovementComponent->HasAnimRootMotion()) {
			CharacterMovementComponent->Velocity = PendingLaunchVelocity + CharacterMovementComponent->GetLastInputVector() * CharacterMovementComponent->MaxCustomMovementSpeed;// 单位按键输入 * 最大自定义移速.
		}
		else {
			CharacterMovementComponent->Velocity = PendingLaunchVelocity + CharacterMovementComponent->ConstrainAnimRootMotionVelocity(CharacterMovementComponent->AnimRootMotionVelocity, CharacterMovementComponent->Velocity);
		}

		/* 安全刷新移动组件(扫过一段距离或者弧边), (移动距离, 排除意外万向锁,)*/
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
	// 	bJump = 1.6f;// 刷新此动作延迟计时.
}

// 启用或设置攀岩.
void UClimbingComponent::Climbing()
{
	SetClimbingState(EMovementMode::MOVE_Custom, ECharacterActionState::CLIMB_STATE, false);
}

// 释放攀爬.
void UClimbingComponent::ReleaseClimbing()
{
	SetClimbingState(EMovementMode::MOVE_Walking, ECharacterActionState::NORMAL_STATE, true);
}

void UClimbingComponent::ClearClimbingState()
{
	ClimbingState = EClimbingState::CLIMBING_NONE;
}

bool UClimbingComponent::IsLowClimbing()
{
	return ClimbingHeight > 40.f;
}

// 是否已切换至攀岩-坠落姿态.
bool UClimbingComponent::IsDropClimbingState()
{
	return ClimbingState == EClimbingState::CLIMBING_DROP;
}

void UClimbingComponent::LaunchCharacter(const FVector& LaunchVelocity)
{
	PendingLaunchVelocity = LaunchVelocity;
}

// 切换为攀岩-坠落状态.
void UClimbingComponent::DropClimbingState()
{
	ClimbingState = EClimbingState::CLIMBING_DROP;
}

/** 监测攀岩的具体射线检测逻辑. */
void UClimbingComponent::TraceClimbingState(float DeltaTime)
{
	FVector ForwardDirection = MMOARPGCharacterBase->GetActorForwardVector();
	FVector UpVectorDirection = MMOARPGCharacterBase->GetActorUpVector();
	FVector LocalLocation = MMOARPGCharacterBase->GetActorLocation();
	FRotator ActorRotation = MMOARPGCharacterBase->GetActorRotation();


	FHitResult HitChestResult;
	float ChestDistance = MaxDistance;// 胸膛到射线相交点的距离.
	{
		// 射线检测 -胸.
		TArray<AActor*> ActorsToIgnore1;
		FVector StartTraceChestLocation = LocalLocation;
// 		StartTraceChestLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight() / 4.0f;// 胸位置略抬高于胶囊体位置.
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
		FVector EndTraceGroundLocation = StartTraceGroundLocation + (-UpVectorDirection) * 40.f;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTraceGroundLocation, EndTraceGroundLocation,
			ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore3,
			EDrawDebugTrace::ForOneFrame, HitGroundResult, true);

		if (HitGroundResult.bBlockingHit) {
			GroundDistance = FVector::Distance(StartTraceGroundLocation, HitGroundResult.Location);
		}
	}


	if (HitChestResult.bBlockingHit && HitHeadResult.bBlockingHit) {/* 两根都命中认为是攀岩. */

		if (ChestDistance <= 45.f /*&& HeadDistance <= 80.f*/) {// 因为胶囊体半径是42. 

			/** 针对半圆球那种情况的修正;让人再稍微前进一步. */
			float CompensationValue = ChestDistance - 29.f;
			if (CompensationValue > 0.f) {
				FVector TargetPoint = ForwardDirection * CompensationValue;
				FVector TargetLocation = MMOARPGCharacterBase->GetActorLocation() + TargetPoint * (DeltaTime * 8.f);// 按帧间隔递进,平滑一下
				MMOARPGCharacterBase->SetActorLocation(TargetLocation);
			}

			/* 攀爬中落地*/
			if (ClimbingState == EClimbingState::CLIMBING_CLIMBING) {

				if (HitGroundResult.bBlockingHit) {
					if (GroundDistance <= 1.f) {
						ClimbingState = EClimbingState::CLIMBING_TOGROUND;
						ReleaseClimbing();// 释放攀爬.						
					}
				}
			}
			/* 攀岩且不落地. */
			else if (ClimbingState != EClimbingState::CLIMBING_TOGROUND && ClimbingState != EClimbingState::CLIMBING_DROP) {
				ClimbingState = EClimbingState::CLIMBING_CLIMBING;// 切位爬高墙枚举.
				Climbing();// 启用攀岩.
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
	else if (HitChestResult.bBlockingHit && !HitHeadResult.bBlockingHit) {/* 只命中胸, 头没命中则视为翻越*/
		/** 爬顶 */
		if (ClimbingState == EClimbingState::CLIMBING_CLIMBING && !HitGroundResult.bBlockingHit) {

			ClimbingState = EClimbingState::CLIMBING_TOTOP;
			GThread::Get()->GetCoroutines().BindLambda(1.f, [this]() {
				ReleaseClimbing();
				});
		}
		/// //////////////////////////////////////////////////////////////////////////
		/** 翻越矮墙 */
		else if (ClimbingState != EClimbingState::CLIMBING_TOTOP &&
			ClimbingState != EClimbingState::CLIMBING_WALLCLIMBING &&
			!bWallClimbing) {
			if (ChestDistance <= 18.f) {
				
				// 1.正面面向朝向矮墙.
				{
					// 类似于FindLookAt
					FRotator NewRot = FRotationMatrix::MakeFromX(MMOARPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();
					ActorRotation.Yaw = NewRot.Yaw;
					ActorRotation.Pitch = 0.0f;
					ActorRotation.Roll = 0.f;// 清空roll.
					MMOARPGCharacterBase->SetActorRotation(ActorRotation);
				}

				// 一条从身体前侧40公分处从上往下打的射线.
				FHitResult HitWallClimbing;
				FVector StartTraceLocation = LocalLocation + ForwardDirection * 40.f;
				StartTraceLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
				FVector EndTraceLocation = StartTraceLocation - UpVectorDirection * 100.f;
				TArray<AActor*> ClimbingActorsToIgnore;
				UKismetSystemLibrary::LineTraceSingle(GetWorld(),
					StartTraceLocation,
					EndTraceLocation,
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					ClimbingActorsToIgnore,
					EDrawDebugTrace::ForDuration, HitWallClimbing, true);

				if (HitWallClimbing.bBlockingHit) {// 当确认有矮墙墙顶
					
					HitWallClimbing.Location.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();// 胶囊体中心点挪到墙顶上头.
					ClimbingTracePoint = HitWallClimbing.Location;
					ClimbingHeight = HitWallClimbing.Distance;
					ClimbingState = EClimbingState::CLIMBING_WALLCLIMBING;// 攀岩状态刷新至翻墙

					AdjustmentClimbing(true);// 微调并优化翻墙后人物的位置.

					if (IsLowClimbing()) {
						bWallClimbing = 0.7f;// 翻矮墙播时长0.8秒
					}
					else { 
						bWallClimbing = 1.7f;// 翻高墙播时长1.7秒
					}
					
					bWallClimbing = true;
				}
			}
		}
	}
	else {

		/** 攀岩中左移或者右移身体有半截对着空气就释放攀岩状态,落地. */
		if (ClimbingState == EClimbingState::CLIMBING_CLIMBING) {
			ClimbingState = EClimbingState::CLIMBING_NONE;
			ReleaseClimbing();// 释放攀爬.
		}
	}

	/** 爬圆柱体. */
	if (HitChestResult.bBlockingHit) {
		if (ClimbingState == EClimbingState::CLIMBING_CLIMBING) {
			// 类似于FindLookAt
			FRotator NewRot = FRotationMatrix::MakeFromX(MMOARPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();
			ActorRotation.Yaw = NewRot.Yaw;
			ActorRotation.Pitch = NewRot.Pitch;
			ActorRotation.Roll = 0.f;// 清空roll.
			MMOARPGCharacterBase->SetActorRotation(ActorRotation);
		}
	}

	/** 翻墙时候人的位置到矮墙顶渐变. */
	if (bWallClimbing) {
// 		MMOARPGCharacterBase->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FVector VInterpToLocation = FMath::VInterpTo(LocalLocation, ClimbingTracePoint, DeltaTime, ClimbToWallTopVInterlerpSpeed);
		MMOARPGCharacterBase->SetActorLocation(VInterpToLocation);
	}

	/** 坠落情况下就清除攀岩. */
	if (ClimbingState == EClimbingState::CLIMBING_DROP) {
		if (CharacterMovementComponent->MovementMode != EMovementMode::MOVE_Falling) {
			ClearClimbingState();
		}
	}
}

// 给一套自定义的攀岩配置.
void UClimbingComponent::SetClimbingState(EMovementMode InMode, ECharacterActionState InCharacterActionState, bool bOrientRotationToMovement)
{
	FRotator ActorRotation = MMOARPGCharacterBase->GetActorRotation();
	CharacterMovementComponent->SetMovementMode(InMode);
	CharacterMovementComponent->bOrientRotationToMovement = bOrientRotationToMovement;
	MMOARPGCharacterBase->ResetActionState(InCharacterActionState);
	ActorRotation.Pitch = 0.0f;
	MMOARPGCharacterBase->SetActorRotation(ActorRotation);// 需要人物朝向pitch也清除掉,以防止落下的时候人物是歪斜的.
	bJumpToClimbing = false;
}

// 微调优化翻墙后的人物位置.
void UClimbingComponent::AdjustmentClimbing(bool bStart /*= true*/)
{
	FVector RelativeLocation = MMOARPGCharacterBase->GetMesh()->GetRelativeLocation();
	float AdjustValue = 10.f;
	if (bStart) {
		RelativeLocation.Z += AdjustValue;
	}
	else {
		RelativeLocation.Z -= AdjustValue;
	}
	MMOARPGCharacterBase->GetMesh()->SetRelativeLocation(RelativeLocation);
}

void UClimbingComponent::AdjustmentPendingLaunchVelocity(float DeltaTime)
{
	auto AxisCheck = [](double& InValue, float DeltaTime) {
		if (FMath::IsNearlyEqual(InValue, 0.f, 1.f)) {// 误差接近0和1就清除误差.
			InValue = 0.f;
		}
		else if (InValue > 0.f) {
			InValue -= InValue * DeltaTime;
		}
		else if (InValue < 0.f) {
			InValue += -InValue * DeltaTime;
		}
	};

	AxisCheck(PendingLaunchVelocity.X, DeltaTime);
	AxisCheck((PendingLaunchVelocity.Y), DeltaTime);
	AxisCheck((PendingLaunchVelocity.Z), DeltaTime);
}

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
	, mTraceLineType(EDrawDebugTrace::None)
	, mTraceLineTypeWhenStepWall(EDrawDebugTrace::None)
{

}

void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	// 翻墙计时结束后绑定的回调
	bWallClimbing.Fun.BindLambda([&]() ->void {
		AdjustmentClimbing(false);
		});

	// 攀岩-拐弯行为计时结束后绑定的回调
	bTurn.Fun.BindLambda([&]() ->void {
		CharacterMovementComponent->GravityScale = 1.f;// 恢复重力.

		// 再打一次射线并用作记录距离.
		FVector ForwardDirection = MMOARPGCharacterBase->GetActorForwardVector();
		FVector LocaLocation = MMOARPGCharacterBase->GetActorLocation();
		FHitResult HitChestResult;
		float ChestDistance = Scanning(HitChestResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
			StartTraceLocation = LocaLocation;
			StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight() / 2.f;
			EndTraceLocation = StartTraceLocation + ForwardDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.f;
			});

		// turn的修正. 让人与墙的距离保持合适的视觉效果.
		if (ChestDistance >= 29.f) {
			LocaLocation += ForwardDirection * (ChestDistance - 29.f);
		}
		else {
			LocaLocation -= ForwardDirection * (29.f - ChestDistance);
		}
		MMOARPGCharacterBase->SetActorLocation(LocaLocation);
		});
}

void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() &&
		CameraComponent.IsValid()) {

		if (CharacterMovementComponent->MovementMode != EMovementMode::MOVE_Flying &&
			MMOARPGCharacterBase->GetActionState() != ECharacterActionState::FIGHT_STATE) {// 战斗姿势里也停止射线检测
			TraceClimbingState(DeltaTime);// 命令其射线检测.

			bJump.Tick(DeltaTime);// Tick 跳爬动作
			bWallClimbing.Tick(DeltaTime);// Tick翻墙.
			bTurn.Tick(DeltaTime);

			AdjustmentPendingLaunchVelocity(DeltaTime);// 调节坠落给的蹬腿反力方向

			if (ClimbingState != EClimbingState::CLIMBING_NONE) {
				UpdateMovement(DeltaTime);// 仅在服务器上为人注入输入方向和输入值.
			}
		}
	}
}

// 覆写组件同步 -GetLifetimeReplicatedProps.
void UClimbingComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 仅同步给模拟玩家, 字段"InputVector"
	DOREPLIFETIME_CONDITION(UClimbingComponent, InputVector, COND_SimulatedOnly);
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

		/* 客户端模拟一次键输输入*/
		MMOARPGCharacterBase->AddMovementInput(Direction, InValue);

		/* 服务器也模拟一次键鼠输入*/
		InputVector = Direction * InValue;
		SetInputVector(InValue, Direction, true);
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

		SetInputVector(InValue, Direction, false);
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

// 还原攀爬系统为爬墙状态.
void UClimbingComponent::ResetClimbingState()
{
	ClimbingState = EClimbingState::CLIMBING_CLIMBING;
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

// RPC至服务器, "根据bRight启用来注册横向或竖向轴的参数"
void UClimbingComponent::SetInputVector_Implementation(float InValue, const FVector& InDirection, bool bRight)
{
	if (bRight) {
		RightInput.Direction = InDirection;
		RightInput.Value = InValue;
	}
	else {
		ForwardInput.Direction = InDirection;
		ForwardInput.Value = InValue;
	}
}

// 仅在服务器上为人注入输入方向和输入值.
void UClimbingComponent::UpdateMovement(float DeltaTime)
{
	if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority) {
		//if (RightInput.Value != 0.f)
		{
			MMOARPGCharacterBase->AddMovementInput(RightInput.Direction, RightInput.Value);
		}
		//	else if(ForwardInput.Value != 0.f)
		{
			MMOARPGCharacterBase->AddMovementInput(ForwardInput.Direction, ForwardInput.Value);
		}
	}
}

/** 监测攀岩的具体射线检测逻辑. */
void UClimbingComponent::TraceClimbingState(float DeltaTime)
{
	// 一旦拐弯跳,必须关闭射线检测.
	if (bTurn) {
		return;
	}

	FVector ForwardDirection = MMOARPGCharacterBase->GetActorForwardVector();
	FVector UpVectorDirection = MMOARPGCharacterBase->GetActorUpVector();
	FVector RightVectorDirection = MMOARPGCharacterBase->GetActorRightVector();
	FVector LocalLocation = MMOARPGCharacterBase->GetActorLocation();
	FRotator ActorRotation = MMOARPGCharacterBase->GetActorRotation();


	FHitResult HitChestResult;
	// 胸膛到射线相交点的距离.
	float ChestDistance = Scanning(HitChestResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
		StartTraceLocation = LocalLocation;
		StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight() / 2.f;
		EndTraceLocation = StartTraceLocation + ForwardDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.f;
		});

	FHitResult HitHeadResult;
	// 头颅到射线相交点的距离.
	float HeadDistance = Scanning(HitHeadResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
		StartTraceLocation = LocalLocation;
		StartTraceLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
		EndTraceLocation = StartTraceLocation + ForwardDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.f;
		});

	FHitResult HitGroundResult;
	// 脚部到射线相交点的距离.
	float GroundDistance = Scanning(HitGroundResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
		StartTraceLocation = LocalLocation;
		StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight();
		EndTraceLocation = StartTraceLocation + (-UpVectorDirection) * 40.f;
		});

	/**
	 * 用以墙角拐弯的四条射线.
	 */
	FHitResult HitRightResult;
	float RightDistance = Scanning(HitRightResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
		StartTraceLocation = LocalLocation + RightVectorDirection * 70.f;
		EndTraceLocation = StartTraceLocation + ForwardDirection * 100.f;
		});
	FHitResult HitLeftSideResult;
	float LeftSideDistance = Scanning(HitLeftSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
		StartTraceLocation = LocalLocation - RightVectorDirection * 10.f - ForwardDirection * 10.f;;
		EndTraceLocation = StartTraceLocation - RightVectorDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});
	FHitResult HitRightSideResult;
	float RightSideDistance = Scanning(HitRightSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
		StartTraceLocation = LocalLocation + RightVectorDirection * 10.f - ForwardDirection * 10.f;;
		EndTraceLocation = StartTraceLocation + RightVectorDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});
	FHitResult HitLeftResult;
	float LeftDistance = Scanning(HitLeftResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
		StartTraceLocation = LocalLocation - RightVectorDirection * 70.f;
		EndTraceLocation = StartTraceLocation + ForwardDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});

	/// 检测内径或者外径拐弯的具体执行Lambda.
	auto CheckTurn = [&]() {
		// 外径,右拐角.
		if (!HitRightResult.bBlockingHit && !HitLeftSideResult.bBlockingHit && !HitRightSideResult.bBlockingHit) {
			FHitResult HitRightClimbingSideResult;
			float RightClimbingSideDistance = Scanning(HitRightClimbingSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
				StartTraceLocation = LocalLocation + RightVectorDirection * 70.f + ForwardDirection * 40.f;
				EndTraceLocation = StartTraceLocation - RightVectorDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
				});

			if (HitRightClimbingSideResult.bBlockingHit && RightClimbingSideDistance != 0.f) {
				if (RightClimbingSideDistance >= 8.f) {
					bTurn = true;
					bTurn = 1.2f;
					TurnState = EClimbingTurnState::OUTSIDE_RIGHT;;
				}
			}
		}
		// 外径,左拐角.
		else if (!HitLeftResult.bBlockingHit && !HitLeftSideResult.bBlockingHit && !HitRightSideResult.bBlockingHit) {
			FHitResult HitLeftClimbingSideResult;
			float LeftClimbingSideDistance = Scanning(HitLeftClimbingSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation) {
				StartTraceLocation = LocalLocation - RightVectorDirection * 70.f + ForwardDirection * 40.f;
				EndTraceLocation = StartTraceLocation + RightVectorDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
				});

			if (HitLeftClimbingSideResult.bBlockingHit) {
				if (LeftClimbingSideDistance >= 8.f && LeftClimbingSideDistance != 0.f) {
					bTurn = true;
					bTurn = 1.2f;
					TurnState = EClimbingTurnState::OUTSIDE_LEFT;
				}
			}
		}
		// 内径,左拐角.
		else if (HitLeftSideResult.bBlockingHit) {
			if (FMath::IsNearlyEqual(LeftSideDistance, 43.f, 0.7f)) {
				bTurn = true;
				bTurn = 1.20f;
				TurnState = EClimbingTurnState::INSIDE_LEFT;
			}
		}
		// 内径, 右拐角.
		else if (HitRightSideResult.bBlockingHit) {
			if (FMath::IsNearlyEqual(RightSideDistance, 49.f, 0.7f)) {// 设计成与47不一致是为了减少反复turn的bug
				bTurn = true;
				bTurn = 1.20f;
				TurnState = EClimbingTurnState::INSIDE_RIGHT;
			}
		}

		// 如若启用拐弯.
		if (bTurn) {
			ClimbingState = EClimbingState::CLIMBING_TURN;
			CharacterMovementComponent->GravityScale = 0.f;
		}
	};


	/// <summary>
	/// 攀岩系统复杂判定.
	/// </summary>
	/// <param name="DeltaTime"></param>
	if (HitChestResult.bBlockingHit && HitHeadResult.bBlockingHit) {/* 两根都命中认为是攀岩. */

		if (ChestDistance <= 45.f /*&& HeadDistance <= 80.f*/) {// 因为胶囊体半径是42. 

			/** 针对半圆球那种情况的修正;让人再稍微前进一步. */
			float CompensationValue = ChestDistance - 29.f;
			if (CompensationValue > 0.f) {
				FVector TargetPoint = ForwardDirection * CompensationValue;
				FVector TargetLocation = MMOARPGCharacterBase->GetActorLocation() + TargetPoint * (DeltaTime * 8.f);// 按帧间隔递进,平滑一下
				MMOARPGCharacterBase->SetActorLocation(TargetLocation);
			}

			/* 开始爬墙*/
			if (ClimbingState == EClimbingState::CLIMBING_CLIMBING) {

				if (HitGroundResult.bBlockingHit) {
					if (GroundDistance <= 1.f) {
						ClimbingState = EClimbingState::CLIMBING_TOGROUND;
						ReleaseClimbing();// 释放攀爬.						
					}
					/** 是否攀岩拐弯; 满足任一条件即可执行 */
					else if (HitRightResult.bBlockingHit ||
						HitLeftResult.bBlockingHit ||
						HitLeftSideResult.bBlockingHit ||
						HitRightSideResult.bBlockingHit) {
						if (!bTurn) {
							CheckTurn();
						}
					}
				}
				// 离地面很搞,检测是否攀岩拐弯.
				else if (
					HitRightResult.bBlockingHit ||
					HitLeftResult.bBlockingHit ||
					HitLeftSideResult.bBlockingHit ||
					HitRightSideResult.bBlockingHit) {
					if (!bTurn) {
						CheckTurn();
					}
				}

				// 爬半圆球的算法 角度调整.
				{
					FVector ZAxis = FVector(0.f, 0.f, 1.0f);
					float CosValue = FVector::DotProduct(ZAxis, HitChestResult.ImpactNormal);
					float CosAngle = (180.f) / PI * FMath::Acos(CosValue);
					if (CosAngle < 35.f) {
						ClimbingState = EClimbingState::CLIMBING_NONE;
						ReleaseClimbing();
					}
					// 					GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, *FString::SanitizeFloat(CosAngle));
				}
			}

			/* 攀岩且不落地. */
			else if (ClimbingState != EClimbingState::CLIMBING_TOGROUND && ClimbingState != EClimbingState::CLIMBING_DROP) {
				ClimbingState = EClimbingState::CLIMBING_CLIMBING;// 切位爬高墙枚举.
				Climbing();// 启用攀岩.

				/** 非常贴近墙面的时候按脚离地面高度 来启用是否切换跳爬 */
				if (HitGroundResult.bBlockingHit == false) {
					bJumpToClimbing = true;// 启用跳攀吸附
				}
				else {
					bWalkToClimbing = true;// 启用步行转攀岩
				}
				//
			}
		}

		/** 离墙面距离大于43就认为用户不再想攀岩. */
		else {
			if (ClimbingState == EClimbingState::CLIMBING_TOGROUND) {
				ClimbingState = EClimbingState::CLIMBING_NONE;
			}
		}
		//
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
				FVector EndTraceLocation = StartTraceLocation - UpVectorDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.f;
				TArray<AActor*> ClimbingActorsToIgnore;
				UKismetSystemLibrary::LineTraceSingle(GetWorld(),
					StartTraceLocation,
					EndTraceLocation,
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					ClimbingActorsToIgnore,
					mTraceLineTypeWhenStepWall, HitWallClimbing, true);

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

	// 前向轴和横向轴清零.
	RightInput.Direction = FVector::ZAxisVector;
	RightInput.Value = 0.f;
	ForwardInput.Direction = FVector::ZAxisVector;
	ForwardInput.Value = 0.f;
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

/** 任意射线检测的封装接口 */
float UClimbingComponent::Scanning(FHitResult& HitResult, TFunction<void(FVector&, FVector&)> TraceLambda)
{
	float ChestDistance = MaxDistance;

	FVector StartTraceLocation, EndTraceLocation;
	TraceLambda(StartTraceLocation, EndTraceLocation);

	TArray<AActor*> ActorsToIgnore;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		StartTraceLocation,
		EndTraceLocation,
		ETraceTypeQuery::TraceTypeQuery1,
		true,
		ActorsToIgnore,
		mTraceLineType, HitResult, true);

	if (HitResult.bBlockingHit) {
		ChestDistance = FVector::Distance(StartTraceLocation, HitResult.Location);
	}

	return ChestDistance;
}

// 解算出合适的攀岩跳枚举.
EClimbingMontageState UClimbingComponent::CalculationClimbingJumpState()
{
	// 	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {
	// 		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent())) {

	// 速度的Pitch,Yaw都存入了1个2D向量.
	// 一定要使用GetLastInputVector() 即使用按键的方向而非运动组件速度的方向.
// 	FVector2D Axis(CharacterMovementComponent->GetLastInputVector().Y, CharacterMovementComponent->GetLastInputVector().Z);

	// 需要转为相对的本地坐标; 一定要使用GetLastInputVector() 即使用按键的方向而非运动组件速度的方向.
	FVector NewInputVector = MMOARPGCharacterBase->GetTransform().InverseTransformPosition(CharacterMovementComponent->GetLastInputVector() + MMOARPGCharacterBase->GetActorLocation());
	//-----------------------------
	FVector2D Axis(NewInputVector.Y, NewInputVector.Z);

	/// 区分左右
	FVector2D XAxis(1.f, 0.f);
	Axis.Normalize();
	float CosValue = FVector2D::DotProduct(Axis, XAxis);
	float XAxisCosAngle = (180.f) / PI * FMath::Acos(CosValue);// 判定横轴, 这个角度是0到180,静止不动的时候为90,向右为0,向左为180

	/// 区分上下
	FVector2D YAxis(0.f, 1.f);
	CosValue = FVector2D::DotProduct(Axis, YAxis);
	float YAxisCosAngle = (180.f) / PI * FMath::Acos(CosValue);// 判定竖轴, 这个角度是0到180,静止不动的时候为90,往上为0,往下为180
	bool bUPAxis = FMath::IsWithinInclusive(YAxisCosAngle, 0.f, 90.f);

	/// 依据横轴度数和 上下半轴为依据, 执行具体蒙太奇类型.
	if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && bUPAxis) {
		return EClimbingMontageState::CLIMBING_DASH_UR_RM;
	}
	else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && !bUPAxis) {
		return EClimbingMontageState::CLIMBING_DASH_DL_RM;
	}
	else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && bUPAxis) {
		return EClimbingMontageState::CLIMBING_DASH_UL_RM;
	}
	else if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && !bUPAxis) {
		return EClimbingMontageState::CLIMBING_DASH_DR_RM;
	}
	else if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f) && bUPAxis) {
		return EClimbingMontageState::CLIMBING_DASH_U_RM;
	}
	else if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f) && !bUPAxis) {
		return EClimbingMontageState::CLIMBING_DASH_D_RM;
	}
	else if (FMath::IsWithinInclusive(XAxisCosAngle, 157.5f, 180.f)) {
		return EClimbingMontageState::CLIMBING_DASH_L_RM;
	}
	else if (FMath::IsWithinInclusive(XAxisCosAngle, 0.f, 22.5f)) {
		return EClimbingMontageState::CLIMBING_DASH_R_RM;
	}
	// 		}
	// 	}

	return EClimbingMontageState::CLIMBING_DASH_MAX;
}

// 广播诸客户端 "复位攀岩跳开关并注册给定蒙太奇"
void UClimbingComponent::MulticastJump_Implementation(EClimbingMontageState InClimbingMontageState)
{
	// 0.刷新复位 攀岩跳
	ResetJump();
	// 1.注册攀岩跳蒙太奇姿势.
	ClimbingMontageState = InClimbingMontageState;
}

// RPC服务器 "MulticastJump"
void UClimbingComponent::JumpToServer_Implementation(EClimbingMontageState InClimbingMontageState)
{
	MulticastJump(InClimbingMontageState);
}

// Jump大逻辑
void UClimbingComponent::Jump()
{
	JumpToServer(CalculationClimbingJumpState());
}

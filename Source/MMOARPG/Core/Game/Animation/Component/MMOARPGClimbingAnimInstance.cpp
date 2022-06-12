// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGClimbingAnimInstance.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Component/ClimbingComponent.h"

void UMMOARPGClimbingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMOARPGClimbingAnimInstance::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {

		// 把组件的各字段关联至动画实例.
		RotationRate = InCharacterBase->GetClimbingComponent()->RotationRate;
		ClimbingState = InCharacterBase->GetClimbingComponent()->ClimbingState;
		bJumpToClimbing = InCharacterBase->GetClimbingComponent()->bJumpToClimbing;
		bJump = *InCharacterBase->GetClimbingComponent()->bJump;// 攀岩跳是否激活.

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent())) {
			ResetAxisSpeed(InCharacterMovementComponent->MaxCustomMovementSpeed);// 最大攀岩速度映射到(-1,1)
		}

		if (bJump) {
			InCharacterBase->GetClimbingComponent()->bJump = false;// 每次跳完都需要重置状态.
			ClimpJump();// 攀岩跳的具体逻辑.
		}



// 		CalculationClimbingJumpState();
	}
}

// 攀岩跳的具体逻辑.
void UMMOARPGClimbingAnimInstance::ClimpJump()
{
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {
		EClimbingJumpState JumpState = CalculationClimbingJumpState();// 先解算一下蒙太奇类型.
		if (JumpState != EClimbingJumpState::CLIMBING_DASH_MAX) {
			InCharacterBase->ClimbingJumpChanged(JumpState);
		}
	}
}

// 解算出合适的攀岩跳枚举.
EClimbingJumpState UMMOARPGClimbingAnimInstance::CalculationClimbingJumpState()
{
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {
		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent())) {
			
			// 速度的Pitch,Yaw都存入了1个2D向量.
			FVector2D Axis(InCharacterMovementComponent->GetLastInputVector().Y, InCharacterMovementComponent->GetLastInputVector().Z);

			/// 区分左右
			FVector2D XAxis(1.f, 0.f);
			Axis.Normalize();
			float CosValue = FVector2D::DotProduct(Axis, XAxis);
			float XAxisCosAngle = (180.f) / PI * FMath::Acos(CosValue);// 判定横轴, 这个角度是0到180,静止不动的时候为90,向右为0,向左为180
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 0.04f, FColor::Yellow, FString::SanitizeFloat(XAxisCosAngle));
			}

			/// 区分上下
			FVector2D YAxis(0.f, 1.f);
			CosValue = FVector2D::DotProduct(Axis, YAxis);
			float YAxisCosAngle = (180.f) / PI * FMath::Acos(CosValue);// 判定竖轴, 这个角度是0到180,静止不动的时候为90,往上为0,往下为180
			bool bUPAxis = FMath::IsWithinInclusive(YAxisCosAngle, 0.2f, 90.f);
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 0.04f, FColor::White, FString::SanitizeFloat(YAxisCosAngle));
			}

			/// 依据横轴度数和 上下半轴为依据, 执行具体蒙太奇类型.
			if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && bUPAxis) {
				return EClimbingJumpState::CLIMBING_DASH_UR_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && !bUPAxis) {
				return EClimbingJumpState::CLIMBING_DASH_DL_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && bUPAxis) {
				return EClimbingJumpState::CLIMBING_DASH_UL_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && !bUPAxis) {
				return EClimbingJumpState::CLIMBING_DASH_DR_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f) && bUPAxis) {
				return EClimbingJumpState::CLIMBING_DASH_U_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f) && !bUPAxis) {
				return EClimbingJumpState::CLIMBING_DASH_D_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 157.5f, 180.f)) {
				return EClimbingJumpState::CLIMBING_DASH_L_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 0.f, 22.5f)) {
				return EClimbingJumpState::CLIMBING_DASH_R_RM;
			}
		}
	}

	return EClimbingJumpState::CLIMBING_DASH_MAX;
}

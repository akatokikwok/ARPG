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

		/* 常规攀岩跳激活检测.*/
		if (bJump) {
			InCharacterBase->GetClimbingComponent()->bJump = false;// 每次跳完都需要重置状态.
			ClimpJump();// 攀岩跳的具体逻辑.
		}

		/* 若符合爬顶状态*/
		if (ClimbingState == EClimbingState::CLIMBING_TOTOP) {
			InCharacterBase->GetClimbingComponent()->ClimbingState = EClimbingState::CLIMBING_NONE;// 状态刷新.
			InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_CLIMB_UP_AT_TOP);// 播蒙太奇.
		}
		/** 翻越任意高度的墙. */
		else if (ClimbingState == EClimbingState::CLIMBING_WALLCLIMBING) {
			// 先清空攀爬.
 			InCharacterBase->GetClimbingComponent()->ClearClimbingState();

			if (InCharacterBase->GetClimbingComponent()->IsLowClimbing()) {// 单手翻矮墙.
				// 随机播一个单手翻矮墙的蒙太奇.
				InCharacterBase->ClimbingMontageChanged(
					(EClimbingMontageState)FMath::RandRange((int32)EClimbingMontageState::CLIMBING_ALS_N_MANTLE_1M_LH, (int32)EClimbingMontageState::CLIMBING_ALS_N_MANTLE_1M_RH));
			}
			else {// 双手翻高墙的蒙太奇.
				InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_ALS_N_MANTLE_2M);
			}
		}
		/** 攀岩拐弯姿态. */
		else if (ClimbingState == EClimbingState::CLIMBING_TURN) {
			EClimbingTurnState TurnState = InCharacterBase->GetClimbingComponent()->GetTurnState();

			switch (TurnState) {
				case EClimbingTurnState::OUTSIDE_RIGHT:
					InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_OUTSIDE_R_RM);
					break;
				case EClimbingTurnState::OUTSIDE_LEFT:
					InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_OUTSIDE_L_RM);
					break;
				case EClimbingTurnState::INSIDE_RIGHT:
					InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_INSIDE_R_RM);
					break;
				case EClimbingTurnState::INSIDE_LEFT:
					InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_INSIDE_L_RM);
					break;
			}
			InCharacterBase->GetClimbingComponent()->ResetClimbingState();
		}
		//
		else if (bJumpToClimbing) {
			InCharacterBase->GetClimbingComponent()->bJumpToClimbing = false;
			InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_FROMAIR_RM);
		}
		//
		else if (InCharacterBase->GetClimbingComponent()->bWalkToClimbing) {
			InCharacterBase->GetClimbingComponent()->bWalkToClimbing = false;
			InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_FROMGROUND_RM);
		}
	}
}

// 攀岩跳的具体逻辑.
void UMMOARPGClimbingAnimInstance::ClimpJump()
{
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {
		EClimbingMontageState JumpState = InCharacterBase->GetClimbingComponent()->GetClimbingMontageState();// 拿到具体的攀岩跳姿势.
		if (JumpState != EClimbingMontageState::CLIMBING_DASH_MAX) {
			InCharacterBase->ClimbingMontageChanged(JumpState);
		}
	}
}

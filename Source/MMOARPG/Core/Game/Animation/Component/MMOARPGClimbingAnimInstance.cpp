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
		bJump = *InCharacterBase->GetClimbingComponent()->bJump;

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent())) {
			ResetAxisSpeed(InCharacterMovementComponent->MaxCustomMovementSpeed);// 最大攀岩速度映射到(-1,1)
		}
	}
}

void UMMOARPGClimbingAnimInstance::ClimpJump()
{
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {
		EClimbingJumpState JumpState;
		{
			// 按照AxisSpeed的轴向速度, 划定一个比较概括的范围去判断爬的时候怎么跳.(播具体哪一段蒙太奇section)
		}
		InCharacterBase->ClimbingJumpChanged(JumpState);
	}
}

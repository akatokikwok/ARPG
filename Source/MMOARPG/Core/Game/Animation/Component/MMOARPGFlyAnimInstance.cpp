﻿// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGFlyAnimInstance.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Component/FlyComponent.h"

UMMOARPGFlyAnimInstance::UMMOARPGFlyAnimInstance()
	: Super()
	, DodgeFly(EDodgeFly::DODGE_NONE)
{

}

void UMMOARPGFlyAnimInstance::InitAnimInstance(ACharacter* InCharacter)
{
	Super::InitAnimInstance(InCharacter);

}

void UMMOARPGFlyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMOARPGFlyAnimInstance::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {

		// 从飞行组件里解算出合适的值去关联动画字段.
		DodgeFly = InCharacterBase->GetFlyComponent()->DodgeFly;
		bLand = *(InCharacterBase->GetFlyComponent()->bLand);
		RotationRate = InCharacterBase->GetFlyComponent()->RotationRate;
		bFast = *(InCharacterBase->GetFlyComponent()->bFast);

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent())) {
			ResetAxisSpeed(InCharacterMovementComponent->MaxFlySpeed);// 最大飞行速度映射到(-1,1)
		}
	}
}

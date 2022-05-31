// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOARPGAnimInstanceBase.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UMMOARPGAnimInstanceBase::UMMOARPGAnimInstanceBase()
	: bDeath(false)
	, Speed(0.0f)
	, bInAir(false)
{

}

void UMMOARPGAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMOARPGAnimInstanceBase::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {// 拿到装载本AnimIns的人物类.
		Speed = InCharacterBase->GetVelocity().Size();// 拿人的速率.
		bInAir = InCharacterBase->GetMovementComponent()->IsFalling();// 空中检测.
	}

}

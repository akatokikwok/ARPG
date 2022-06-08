// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGSwimmingAnimInstance.h"
#include "../../../Component/SwimmingComponent.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

void UMMOARPGSwimmingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMOARPGSwimmingAnimInstance::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {

		// 把组件的各字段关联至动画实例.
		bFast = *InCharacterBase->GetSwimmingComponent()->bFast;
		bDiving = *InCharacterBase->GetSwimmingComponent()->bDiving;
		RotationRate = InCharacterBase->GetSwimmingComponent()->RotationRate;

	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstanceComponentBase.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Component/FlyComponent.h"

void UAnimInstanceComponentBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UAnimInstanceComponentBase::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {
		FVector SpeedVector = InCharacterBase->GetVelocity();// 人实际速度,带方向.

		/* 合理视觉需求: 即当无论如何旋转,都希望人体方向向着正向轴.*/
		/* UnrotateVector: 负责把旋转后的向量的 参照系也按照rot执行匹配旋转, 从而达到合理的视觉效果.*/
		FRotator CharacterRot = InCharacterBase->GetActorRotation();
		SpeedVector = CharacterRot.UnrotateVector(SpeedVector);
		// 		UE_LOG(LogTemp, Warning, TEXT("当前人速度是 %f, %f, %f"), SpeedVector.X, SpeedVector.Y, SpeedVector.Z);

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent())) {
			float MaxFlySpeed = InCharacterMovementComponent->MaxFlySpeed;// 人最大飞行速度.

			/* 将最大速度归一化映射到(-1,1),方便动态的模拟Blendspace动画.*/
			AxisSpeed.X = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFlySpeed, MaxFlySpeed), FVector2D(-1.f, 1.f), SpeedVector.X);
			AxisSpeed.Y = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFlySpeed, MaxFlySpeed), FVector2D(-1.f, 1.f), SpeedVector.Y);
			AxisSpeed.Z = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFlySpeed, MaxFlySpeed), FVector2D(-1.f, 1.f), SpeedVector.Z);
		}

		// 从飞行组件里解算出合适的值去关联动画字段.
		RotationRate = InCharacterBase->GetFlyComponent()->RotationRate;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionComponent.h"
#include "../../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UMotionComponent::UMotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotionComponent::BeginPlay()
{
	Super::BeginPlay();
	MMOARPGCharacterBase = Cast<AMMOARPGCharacterBase>(GetOwner());
	if (MMOARPGCharacterBase.IsValid()) {
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(MMOARPGCharacterBase->GetMovementComponent());
		CapsuleComponent = MMOARPGCharacterBase->GetCapsuleComponent();
		CameraComponent = MMOARPGCharacterBase->GetFollowCamera();
	}
}


// Called every frame
void UMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 急速 执行某运动动作.
	bFast.Tick(DeltaTime);
}

void UMotionComponent::LockView(float DeltaTime, bool bClearPitch)
{
	FRotator CameraRotator = CameraComponent->GetComponentRotation();
	FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();

	if (!bClearPitch) {
		CameraRotator.Pitch = 0.0f;// 慢速飞行姿态下 不需要相机转动角的pitch参与计算,故清空.
	}

	/* 迫使 胶囊体旋转朝向 以插值形式快速近似 观察相机朝向.*/
	FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator, DeltaTime, 8.0f);
	MMOARPGCharacterBase->SetActorRotation(NewRot);
}

void UMotionComponent::ResetRotationRate(float DeltaTime)
{
	if (1) {/* 第一种算法*/

		float PreFrameNum = 1.f / DeltaTime;// 单秒帧数.
		FRotator NewDeltaTimeRot = MMOARPGCharacterBase->GetActorRotation() - LastRotator;// 单帧转动弧度.
		NewDeltaTimeRot *= PreFrameNum;// 一秒内转过的弧度.
		//Print(DeltaTime, NewDeltaTimeRot.ToString());
		RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1.f, 1.f), NewDeltaTimeRot.Yaw);
		RotationRate.Y = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1.f, 1.f), NewDeltaTimeRot.Pitch);
		LastRotator = MMOARPGCharacterBase->GetActorRotation();

	}
	else {/*第二种思路,备用*/

		/* 设置角速度(yaw上正负360度)并映射到混合空间里的人物头转向的的(-1,1)*/
		FVector  PhysicsAngularVelocityInDegrees = CapsuleComponent->GetPhysicsAngularVelocityInDegrees();// 通过胶囊体拿角速度.
// 		Print(DeltaTime, PhysicsAngularVelocityInDegrees.ToString());
		RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1, 1),
			PhysicsAngularVelocityInDegrees.Z// 绕Z就是Yaw
		);
		RotationRate.Y = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1, 1),
			PhysicsAngularVelocityInDegrees.X// 绕x就是Pitch.
		);
	}
}


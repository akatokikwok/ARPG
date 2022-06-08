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


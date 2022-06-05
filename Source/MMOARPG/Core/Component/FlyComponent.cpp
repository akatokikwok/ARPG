﻿// Fill out your copyright notice in the Description page of Project Settings.
#include "FlyComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UFlyComponent::UFlyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UFlyComponent::BeginPlay()
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
void UFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CharacterMovementComponent.IsValid() && MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() && CameraComponent.IsValid()) {
		if (MMOARPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE) {
			
			FRotator CameraRotator = CameraComponent->GetComponentRotation();
			FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();

			if (1) {/* 第一种算法*/
				
				/* 迫使 胶囊体旋转朝向 以插值形式快速近似 观察相机朝向.*/
				FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator, DeltaTime, 8.0f);
				MMOARPGCharacterBase->SetActorRotation(NewRot);

				/* 设置角速度(yaw上正负360度)并映射到混合空间里的人物头转向的的(-1,1)*/
				FVector  PhysicsAngularVelocityInDegrees = CapsuleComponent->GetPhysicsAngularVelocityInDegrees();// 通过胶囊体拿角速度.
				Print(DeltaTime, PhysicsAngularVelocityInDegrees.ToString());
				RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1, 1),
					PhysicsAngularVelocityInDegrees.Z// 绕Z就是Yaw
				);
			}
			else {/*第二种思路,备用*/

				float PreFrameNum = 1.f / DeltaTime;// 单秒帧数.
				FRotator NewDeltaTimeRot = MMOARPGCharacterBase->GetActorRotation() - LastRotator;// 单帧转动弧度.
				NewDeltaTimeRot *= PreFrameNum;// 一秒内转过的弧度.
				//Print(DeltaTime, NewDeltaTimeRot.ToString());
				RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1.f, 1.f), NewDeltaTimeRot.Yaw);
				RotationRate.Y = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1.f, 1.f), NewDeltaTimeRot.Pitch);
				LastRotator = MMOARPGCharacterBase->GetActorRotation();
			}
			//			
		}
	}

}

void UFlyComponent::ResetFly()
{
	if (CharacterMovementComponent.IsValid() && MMOARPGCharacterBase.IsValid()) {
		/**
		 * 需要在standAlone模式下去测试
		 * 因为SetMovementMode和网络有关系,standalone是服务器与客户端连为一体.
		 */

		if (MMOARPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE) {/*飞行姿态下*/
			CharacterMovementComponent->bOrientRotationToMovement = false;// 禁用随移动组件旋转.
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);// 切换为UE提供的飞行MODE,(即此MODE下无法跳等其他操作)
		}
		else {/*非飞行姿态.*/
			CharacterMovementComponent->bOrientRotationToMovement = true;// 开启随移动组件旋转.
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);// 切换为UE提供的 Walking Mode.
		}
		// 		bFastFly = false;
	}
}

//////////////////////////////////////////////////////////////////////////

void UFlyComponent::Reset()
{
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->MaxFlySpeed = 600.f;
	CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);

	FRotator NewRot = MMOARPGCharacterBase->GetActorRotation();
	NewRot.Pitch = 0.f;
	MMOARPGCharacterBase->SetActorRotation(NewRot);
}

void UFlyComponent::Print(float InTime, const FString& InString)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, InTime, FColor::Red, FString::Printf(TEXT("%s"), *InString));
	}
}


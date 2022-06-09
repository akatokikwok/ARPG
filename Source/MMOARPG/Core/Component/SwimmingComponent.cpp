// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimmingComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

void USwimmingComponent::BeginPlay()
{
	Super::BeginPlay();

}

void USwimmingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CharacterMovementComponent.IsValid()) {

		if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming
			&& MMOARPGCharacterBase->GetActionState() != ECharacterActionState::SWIMMING_STATE) {

			MMOARPGCharacterBase->ResetActionState(ECharacterActionState::SWIMMING_STATE);
		}
		else if (CharacterMovementComponent->MovementMode != EMovementMode::MOVE_Swimming
			&& MMOARPGCharacterBase->GetActionState() == ECharacterActionState::SWIMMING_STATE) {

			MMOARPGCharacterBase->ResetActionState(ECharacterActionState::SWIMMING_STATE);
		}
		else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming
			&& MMOARPGCharacterBase->GetActionState() == ECharacterActionState::SWIMMING_STATE) {
			/** 仅当完全进入游泳行为才执行真正逻辑. */
			
			LockView(DeltaTime, *bDiving);

			ResetRotationRate(DeltaTime);// 在单帧内矫正并映射(-1,1) 的角速度(yaw轴 pitch轴)
		}

	}
}

// 接收具体运动方向的键盘输入.
void USwimmingComponent::SwimForwardAxis(float InVlaue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() &&
		CameraComponent.IsValid()) {
		if (InVlaue >= 0.f) {
			
			if (bDiving) {
				const FVector Direction = CameraComponent->GetForwardVector();
				MMOARPGCharacterBase->AddMovementInput(Direction, InVlaue);// 按相机指向的方向进行输入移动.
			}
			else {
				if (AController* Controller = MMOARPGCharacterBase->GetController()) {
					// find out which way is forward
					const FRotator Rotation = Controller->GetControlRotation();
					const FRotator YawRotation(0, Rotation.Yaw, 0);
					// get forward vector
					const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
					MMOARPGCharacterBase->AddMovementInput(Direction, InVlaue);
				}
			}
			
		}
	}
}

void USwimmingComponent::ResetFastSwiming()
{
	if (CharacterMovementComponent.IsValid()) {
		if (bFast) {
			bFast = false;
			CharacterMovementComponent->MaxSwimSpeed = 300.f;
		}
		else {
			bFast = true;
			CharacterMovementComponent->MaxSwimSpeed = 600.f;
		}
	}
}

void USwimmingComponent::GoUnderWater()
{
	if (bDiving) {
		bDiving = false;

		if (bFast) {
			CharacterMovementComponent->MaxSwimSpeed = 600.f;
		}
		else {
			CharacterMovementComponent->MaxSwimSpeed = 300.f;
		}

 		CharacterMovementComponent->RotationRate = FRotator(0.f, 0.f, 300.f);// CharacterMovement里的Rotation Rate字段负责控制人物转动的阻滞感.
	}
	else {
		bDiving = true;
		CharacterMovementComponent->MaxSwimSpeed = 600.f;

		CharacterMovementComponent->RotationRate = FRotator(0.f, 0.f, 540.f);// CharacterMovement里的Rotation Rate字段负责控制人物转动的阻滞感.
	}
}

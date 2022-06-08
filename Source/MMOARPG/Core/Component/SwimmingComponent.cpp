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

		}

	}
}

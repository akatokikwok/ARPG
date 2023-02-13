// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotifyState/AnimNotifyState_StopRotation.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotifyState_StopRotation::UAnimNotifyState_StopRotation()
	:SaveStopRotation(FRotator::ZeroRotator)
{

}

void UAnimNotifyState_StopRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		//SaveStopRotation = InCharacter->GetCharacterMovement()->MaxWalkSpeed;
		SaveStopRotation = InCharacter->GetCharacterMovement()->RotationRate;
		InCharacter->GetCharacterMovement()->RotationRate = FRotator::ZeroRotator;
	}
}

void UAnimNotifyState_StopRotation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

}

void UAnimNotifyState_StopRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		InCharacter->GetCharacterMovement()->RotationRate = SaveStopRotation;
	}
}

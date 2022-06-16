// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/AnimNotifyState_StopAnimMontage.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
void UAnimNotifyState_StopAnimMontage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

}

void UAnimNotifyState_StopAnimMontage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		if (InCharacter->GetCharacterMovement()->Velocity.Size() >= 5.f) {
			InCharacter->StopAnimMontage();
		}
	}
}

void UAnimNotifyState_StopAnimMontage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

}

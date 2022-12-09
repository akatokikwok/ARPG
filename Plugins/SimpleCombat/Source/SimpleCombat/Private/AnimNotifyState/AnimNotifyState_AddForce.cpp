// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/AnimNotifyState_AddForce.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotifyState_AddForce::UAnimNotifyState_AddForce()
	: Super()
	, DirectionForce(1.f, 0.f, 0.f)
	, ForceSize(1800000.f)
	, TotalDurationConsuming(0.f)
	, ForceSizeConsuming(0.f)
{

}

void UAnimNotifyState_AddForce::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	TotalDurationConsuming = TotalDuration;// 记录一下总时长.
	ForceSizeConsuming = ForceSize;// 记录/拷贝一份总力大小.
}

void UAnimNotifyState_AddForce::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (TotalDurationConsuming > 0.0f) {
		if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
			// 给人施加推力.
			FVector NewDirection = CalCurrentCharacterDirection(InCharacter);
			InCharacter->GetCharacterMovement()->AddForce(ForceSizeConsuming * NewDirection);

			if (ForceSizeConsuming > 0.0f) {
				// 每秒帧数
				float PreSecondFrame = 1.f / FrameDeltaTime;
				// 这段时间的总消耗帧数
				float TotalFrame = TotalDurationConsuming * PreSecondFrame;
				// 单帧分摊到的力.
				float ForceAttenuationFrameDeltaTime = ForceSize / TotalFrame;
				// 从总力里逐帧扣除单帧力.
				ForceSizeConsuming -= ForceAttenuationFrameDeltaTime;
			}
		}
	}
}

void UAnimNotifyState_AddForce::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

}

FVector UAnimNotifyState_AddForce::CalCurrentCharacterDirection(ACharacter* InCharacter)
{
	if (InCharacter) {
		if (DirectionForce.X != 0.f) {
			return InCharacter->GetActorForwardVector() * DirectionForce.X;
		}
		else if (DirectionForce.Y != 0.f) {
			return InCharacter->GetActorRightVector() * DirectionForce.Y;
		}
		else if (DirectionForce.Z != 0.f) {
			return InCharacter->GetActorUpVector() * DirectionForce.Z;
		}
	}
	return FVector::ZeroVector;
}

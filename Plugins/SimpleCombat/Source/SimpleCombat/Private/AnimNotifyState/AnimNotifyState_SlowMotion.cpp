// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState/AnimNotifyState_SlowMotion.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UAnimNotifyState_SlowMotion::UAnimNotifyState_SlowMotion()
	:Super()
	, Speed(0.2f)
{

}

void UAnimNotifyState_SlowMotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReferencen)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReferencen);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		if (InCharacter->GetWorld() && !InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
			/** 设置全局时间膨胀(Speed是膨胀倍率) */
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Speed);
		}
	}
}

void UAnimNotifyState_SlowMotion::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReferencen)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReferencen);
}

void UAnimNotifyState_SlowMotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReferencen)
{
	Super::NotifyEnd(MeshComp, Animation, EventReferencen);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		if (InCharacter->GetWorld() && !InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
			/** 复位还原 设置全局时间膨胀(Speed是膨胀倍率) */
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		}
	}
}
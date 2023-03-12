// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_StopRotation.generated.h"

/**
 * 此NS在开启时消除pawn的转向, 结束时恢复pawn的转向角.
 */
UCLASS()
class SIMPLECOMBAT_API UAnimNotifyState_StopRotation : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_StopRotation();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

private:
	FRotator SaveStopRotation;
};

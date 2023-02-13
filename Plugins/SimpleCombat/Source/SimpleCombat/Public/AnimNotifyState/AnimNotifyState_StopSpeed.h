// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_StopSpeed.generated.h"

/**
 * 此NS在开启时冻结pawn的移动速度,结束时候会恢复pawn的移动速度
 */
UCLASS(meta = (DisplayName = "StopSpeed"))
class SIMPLECOMBAT_API UAnimNotifyState_StopSpeed : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_StopSpeed();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
private:
	float MaxWalkSpeed;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_StopAnimMontage.generated.h"

/**
 * AnimNotifyState_StopAnimMontage: 用于停止最后的滑步.
 */
UCLASS(meta = (DisplayName = "StopAnimMontage"))
class SIMPLECOMBAT_API UAnimNotifyState_StopAnimMontage : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuratio, const FAnimNotifyEventReference& EventReferencen) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

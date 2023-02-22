// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SlowMotion.generated.h"

/**
 * 慢动作/子弹时间的AnimeNotifyState
 */
UCLASS(meta = (DisplayName = "SlowMotion"))
class SIMPLECOMBAT_API UAnimNotifyState_SlowMotion : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_SlowMotion();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReferencen);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReferencen);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReferencen);

public:
	// 全局时间膨胀倍率.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotifyState_SlowMotion")
		float Speed;
};

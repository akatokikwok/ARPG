// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StopAnimMontage.generated.h"

/**
 * 此Notify会使正在播放的蒙太奇中断.
 */
UCLASS(meta = (DisplayName = "StopAnimMontage"))
class SIMPLECOMBAT_API UAnimNotify_StopAnimMontage : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_StopAnimMontage();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};

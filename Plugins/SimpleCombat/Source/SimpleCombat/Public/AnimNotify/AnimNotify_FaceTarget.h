// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FaceTarget.generated.h"

/**
 * Notify: 让施法者朝向面向敌人目标
 */
UCLASS(meta = (DisplayName = "FaceTarget"))
class SIMPLECOMBAT_API UAnimNotify_FaceTarget : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_FaceTarget();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face Target")
		bool bYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face Target")
		bool bPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face Target")
		bool bRoll;
};

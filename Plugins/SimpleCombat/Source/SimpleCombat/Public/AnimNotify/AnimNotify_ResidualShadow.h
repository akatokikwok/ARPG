// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ResidualShadow.generated.h"

class AResidualShadowActor;

/**
 * 动画通知:闪避残影
 */
UCLASS(meta = (DisplayName = "ResidualShadow"))
class SIMPLECOMBAT_API UAnimNotify_ResidualShadow : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_ResidualShadow();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	// 闪避残影TSub
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		TSubclassOf<AResidualShadowActor> ResidualShadowClass;

	// 闪避残影存续时长
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		float ResidualShadowLifeTime;
};

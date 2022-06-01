// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AnimSignal.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLECOMBAT_API UAnimNotify_AnimSignal : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_AnimSignal();

	// 重写GetNotifyName_Implementation.
	virtual FString GetNotifyName_Implementation() const override;
	// 重写Notify.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	// 信号值.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		int32 SignalValue;
	
};

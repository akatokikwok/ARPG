// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AddForce.generated.h"

class ACharacter;
/**
 * 用来模拟衰减力的
 * 继承自引擎类UAnimNotifyState
 */
UCLASS(meta = (DisplayName = "AddForce"))
class SIMPLECOMBAT_API UAnimNotifyState_AddForce : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_AddForce();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
private:
	// 解算出推力的方向.
	FVector CalCurrentCharacterDirection(ACharacter* InCharacter);

public:
	// 力的方向,由用户配置.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotifyState_AddForce")
		FVector DirectionForce;
	// 力的大小,由用户配置.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotifyState_AddForce")
		float ForceSize;
private:
	float TotalDurationConsuming;// 总衰减时长.
	float ForceSizeConsuming;// 总力尺寸.
};

﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MMOARPGAnimInstanceBase.generated.h"

/**
 *
 */
UCLASS()
class MMOARPG_API UMMOARPGAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMMOARPGAnimInstanceBase();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;

public:
	// 是否死亡.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bDeath;
	// 人的速率.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		float Speed;
	// 是否滞空.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bInAir;
	// 是否战斗姿势.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bFight;
	// 
	// 	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	// 	bool bFootIK;
	// 
	// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	// 	float LeftOffset;
	// 
	// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	// 	float RightOffset;
	// 
	// 	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	// 	FName LeftBoneName;
	// 
	// 	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	// 	FName RightBoneName;
	// 
	// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	// 	float ButtZOffset;
	// 
	// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "AnimAttrubute|FootIK")
	// 	int32 FookIKID;
	// 
	// 	//保留 
	// 	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	// 	TArray<FName> BoneNames;
	// public:
	// 	UFUNCTION(BlueprintPure, BlueprintCallable)
	// 	float GetFootIKOffset(const FName &InBoneName);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Instance/Core/MMOARPGAnimInstanceBase.h"
#include "Core/AnimInstanceComponentBase.h"
#include "MMOARPGFlyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UMMOARPGFlyAnimInstance : public UAnimInstanceComponentBase
{
	GENERATED_BODY()
public:
	UMMOARPGFlyAnimInstance();
	virtual void InitAnimInstance(ACharacter* InCharacter) override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;

public:
	// 是否启用急速飞行.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bFastFly;

	// 加速飞行时, 空中翻滚种类.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EDodgeFly DodgeFly;

	// 是否着陆.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bLand;
};

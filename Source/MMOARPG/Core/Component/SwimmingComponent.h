// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "SwimmingComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API USwimmingComponent : public UMotionComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 接收具体运动方向的键盘输入.
	void SwimForwardAxis(float InVlaue);
};

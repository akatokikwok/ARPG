// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../MMOARPGGameType.h"
#include "MotionComponent.generated.h"

class AMMOARPGCharacterBase;
class UCharacterMovementComponent;
class UCapsuleComponent;
class UCameraComponent;

/**
 * 控制运动行为的组件(如飞行,游泳,攀爬)
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMOARPG_API UMotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMotionComponent();
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 锁定视角.
	void LockView(float DeltaTime, bool bClearPitch = false);
	// 在单帧内矫正并映射(-1,1) 的角速度(yaw轴 pitch轴)
	void ResetRotationRate(float DeltaTime);

	/// //////////////////////////////////////////////////////////////////////////
public:
	// 是否加速/急速 执行某种运动动作.
	FResetBool bFast;

	// 控制某种姿态中人在yaw轴上的朝向.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector2D RotationRate;
protected:
	/**
	 * TWeakObjectPtr是UnrealEngine中UObject型的WeakPtr，其作用：
	 * 可以忽略一个对象是否还有效的判断情况下，直接使用该对象，而程序安全执行.
	 * 比如我拿到了AI对象，但不确定该AI是否被其他玩家打死，这里就可以使用弱指针.
	 */
	UPROPERTY()
		TWeakObjectPtr<AMMOARPGCharacterBase> MMOARPGCharacterBase;

	/* 由于人物所有组件都附着在胶囊体下,所以保存1个胶囊体.*/
	UPROPERTY()
		TWeakObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY()
		TWeakObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
		TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	/* 上一帧人物转向.*/
	UPROPERTY()
		FRotator LastRotator;

};

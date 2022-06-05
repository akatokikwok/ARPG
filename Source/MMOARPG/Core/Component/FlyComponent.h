// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../MMOARPGGameType.h"
#include "FlyComponent.generated.h"

class AMMOARPGCharacterBase;
class UCharacterMovementComponent;
class UCapsuleComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMOARPG_API UFlyComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UFlyComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 还原一套用于飞行姿态下的组件设置.
	void ResetFly();
protected:
	//
	void Reset();
	// 打印指定时长的指定语句.
	void Print(float InTime, const FString& InString);

public:
	// 控制飞行姿态中人在yaw朝向.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector2D RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EDodgeFly DodgeFly;
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

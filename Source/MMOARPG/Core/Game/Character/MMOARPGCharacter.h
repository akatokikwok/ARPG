// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/MMOARPGCharacterBase.h"
#include "MMOARPGCharacter.generated.h"

UCLASS(config=Game)
class AMMOARPGCharacter : public AMMOARPGCharacterBase/*, public IKneadingInterface*/
{
	GENERATED_BODY()
public:
	AMMOARPGCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//////////////////////////////////////////////////////////////////////////

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** 覆写虚接口, 拿附属的相机. **/
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// 打印指定时长的指定语句.
	void Print(float InTime, const FString& InString);
	/** Resets HMD orientation in VR. */
	void OnResetVR();
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector InLocation);
	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector InLocation);

	/** 切换战斗姿势启用/禁用. */
	void SwitchFight();
	
	/** 变量bFight在DS被修改之后发生的逻辑; 可供OnRep_FightChanged调用. */
	void FightChanged();
	/** 当字段被DS刷新后,本机/其他的客户端Player做出的反应. */
	virtual void OnRep_ActionStateChanged() override;

	/* 飞行. */
	UFUNCTION()
		void Fly();
	/* 急速飞行.*/
	UFUNCTION()
		void Fast();
	/* 空中左翻滚.*/
	UFUNCTION()
		void DodgeLeft();
	/* 空中右翻滚.*/
	UFUNCTION()
		void DodgeRight();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

};


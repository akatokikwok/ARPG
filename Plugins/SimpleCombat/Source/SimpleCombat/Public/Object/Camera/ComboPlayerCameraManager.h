// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraModifier_CameraShake.h"
#include "ComboPlayerCameraManager.generated.h"

class UCameraShakeBase;

/**
 * 负责连击的相机管理类
 */
UCLASS()
class SIMPLECOMBAT_API AComboPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	/** 执行相机震动 */
	virtual UCameraShakeBase* PlayCameraShake(
		TSubclassOf<UCameraShakeBase> InShakeClass,
		FOnInitializeCameraShake OnInitializeCameraShake = FOnInitializeCameraShake(),
		float Scale = 1.f,
		ECameraShakePlaySpace PlaySpace = ECameraShakePlaySpace::CameraLocal,
		FRotator UserPlaySpaceRot = FRotator::ZeroRotator);
};

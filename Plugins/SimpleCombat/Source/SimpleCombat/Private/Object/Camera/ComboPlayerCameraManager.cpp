// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Camera/ComboPlayerCameraManager.h"

UCameraShakeBase* AComboPlayerCameraManager::PlayCameraShake(TSubclassOf<UCameraShakeBase> InShakeClass, FOnInitializeCameraShake OnInitializeCameraShake, float Scale, ECameraShakePlaySpace PlaySpace, FRotator UserPlaySpaceRot)
{
	if (InShakeClass && CachedCameraShakeMod && (Scale > 0.0f)) {
		FAddCameraShakeParams Params(Scale, PlaySpace, UserPlaySpaceRot);
		Params.Initializer = OnInitializeCameraShake;

		return CachedCameraShakeMod->AddCameraShake(InShakeClass, Params);
	}

	return nullptr;
}
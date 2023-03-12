// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Camera/ComboPlayerCameraManager.h"

UCameraShakeBase* AComboPlayerCameraManager::PlayCameraShake(
	TSubclassOf<UCameraShakeBase> InShakeClass, 
	FOnInitializeCameraShake OnInitializeCameraShake, // 注册外界给的相机震动代理
	float Scale, 
	ECameraShakePlaySpace PlaySpace,
	FRotator UserPlaySpaceRot)
{
	if (InShakeClass && APlayerCameraManager::CachedCameraShakeMod && (Scale > 0.0f)) {
		
		// 组1个相机震动参数包
		FAddCameraShakeParams Params(Scale, PlaySpace, UserPlaySpaceRot);
		// 为参数包制定1个相机震动委托
		Params.Initializer = OnInitializeCameraShake;

		return CachedCameraShakeMod->AddCameraShake(InShakeClass, Params);
	}

	return nullptr;
}
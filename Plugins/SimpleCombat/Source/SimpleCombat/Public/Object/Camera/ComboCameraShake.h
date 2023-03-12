// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "ComboCameraShake.generated.h"

/**
 *
 */
UCLASS()
class SIMPLECOMBAT_API UComboCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()
public:

	UComboCameraShake(const FObjectInitializer& ObjInit);
};

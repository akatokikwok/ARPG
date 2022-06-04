﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MMOARPGType.h"
#include "MMOARPGPlayerState.generated.h"

/**
 * PS.
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FMMOARPGCharacterAppearance& GetCA() { return CA; }

private:
	FMMOARPGCharacterAppearance CA;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MMOARPGAIControllerBase.generated.h"

class AMMOARPGCharacterBase;
/**
 *
 */
UCLASS()
class MMOARPG_API AMMOARPGAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};

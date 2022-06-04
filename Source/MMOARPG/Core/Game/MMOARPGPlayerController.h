// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MMOARPGPlayerController.generated.h"

class AMMOARPGCharacter;
/**
 *
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMMOARPGPlayerController();
	
	// 运行在服务器.
	UFUNCTION(server, reliable)
		void ReplaceCharacter(int32 InCharacterID);
};

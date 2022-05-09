// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HallHUD.generated.h"

class UUI_HallMain;

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallHUD : public AHUD
{
	GENERATED_BODY()
public:
	AHallHUD();
	virtual void BeginPlay() override;

private:
	UUI_HallMain* HallMain;

private:
	TSubclassOf<UUI_HallMain> HallMainClass;
};

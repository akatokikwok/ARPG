// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HotUpdateHUD.generated.h"

class UUI_HotUpdateMain;

/**
 * 
 */
UCLASS()
class MMOARPG_API AHotUpdateHUD : public AHUD
{
	GENERATED_BODY()
public:
	AHotUpdateHUD();
	virtual void BeginPlay() override;

private:
	UUI_HotUpdateMain* HotUpdateMain;

private:
	TSubclassOf<UUI_HotUpdateMain> HotUpdateMainClass;
};

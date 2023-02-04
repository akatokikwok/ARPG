// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Common/Core/GamePlay/HUDBase.h"
#include "LoginHUD.generated.h"

class UUI_LoginMain;

/**
 * 
 */
UCLASS()
class MMOARPG_API ALoginHUD : public AHUDBase
{
	GENERATED_BODY()
public:
	ALoginHUD();
	virtual void BeginPlay() override;

private:
	UUI_LoginMain* LoginMain;

private:
	TSubclassOf<UUI_LoginMain> LoginMainClass;
};

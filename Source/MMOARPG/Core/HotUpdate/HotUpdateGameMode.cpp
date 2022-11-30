// Fill out your copyright notice in the Description page of Project Settings.


#include "HotUpdateGameMode.h"
#include "HotUpdateHUD.h"
#include "HotUpdatePawn.h"
#include "HotUpdatePlayerController.h"

AHotUpdateGameMode::AHotUpdateGameMode()
{
	HUDClass = AHotUpdateHUD::StaticClass();
	DefaultPawnClass = AHotUpdatePawn::StaticClass();
	PlayerControllerClass = AHotUpdatePlayerController::StaticClass();
}

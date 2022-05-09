// Fill out your copyright notice in the Description page of Project Settings.


#include "HallGameMode.h"
#include "HallHUD.h"
#include "HallPawn.h"
#include "HallPlayerController.h"

AHallGameMode::AHallGameMode()
{
	HUDClass = AHallHUD::StaticClass();
	DefaultPawnClass = AHallPawn::StaticClass();
	PlayerControllerClass = AHallPlayerController::StaticClass();
}

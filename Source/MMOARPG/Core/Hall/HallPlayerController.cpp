﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HallPlayerController.h"

AHallPlayerController::AHallPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;// 允许敲击事件.
	bEnableTouchEvents = true;// 允许触摸事件.
}

void AHallPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AHallPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHallPlayerController::ExecutionRotateCharacter()
{
	SimpleBrowse.OpenBrowsing();
}

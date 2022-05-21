// Fill out your copyright notice in the Description page of Project Settings.


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
	SimpleBrowse.Register(this, nullptr);// 为浏览器注册controller和目标.

}

void AHallPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// 松开左键->停止浏览.
	InputComponent->BindAction("MouseClick", IE_Released, this, &AHallPlayerController::StopRotateCharacter);
	
	InputComponent->BindAction("MouseRightClick", IE_Pressed, this, &AHallPlayerController::BeginMove_callback);
	InputComponent->BindAction("MouseRightClick", IE_Released, this, &AHallPlayerController::EndMove_callback);
}

void AHallPlayerController::ExecutionRotateCharacter()
{
	SimpleBrowse.OpenBrowsing();
}

void AHallPlayerController::StopRotateCharacter()
{
	SimpleBrowse.EndBrowsing();
}

void AHallPlayerController::ResetTarget(AActor* InTarget)
{
	SimpleBrowse.ResetTarget(InTarget);
}

void AHallPlayerController::Zoom(float InDeltaTime)
{

}

void AHallPlayerController::BeginMove_callback()
{

}

void AHallPlayerController::EndMove_callback()
{

}

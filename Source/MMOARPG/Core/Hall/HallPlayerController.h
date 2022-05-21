// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/SimpleBrowse.h"
#include "HallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//
	AHallPlayerController();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** 启用浏览. */
	void ExecutionRotateCharacter();


private:
	SimpleActorAction::FSimpleBrowse SimpleBrowse;// 3D物体浏览器.
	
};

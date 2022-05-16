// Fill out your copyright notice in the Description page of Project Settings.


#include "HallPawn.h"

// Sets default values
AHallPawn::AHallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CharacterStage = nullptr;// 待创建的舞台角色设为空.
}

// Called when the game starts or when spawned
void AHallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHallPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (CharacterStage != nullptr) {
		CharacterStage->Destroy();
	}
}

// Called every frame
void AHallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


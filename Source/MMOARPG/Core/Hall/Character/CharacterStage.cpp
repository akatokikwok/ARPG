// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStage.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterStage::ACharacterStage()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterStage::BeginPlay()
{
	Super::BeginPlay();
	if (GetCapsuleComponent() != nullptr) {
		GetCapsuleComponent()->OnClicked.AddDynamic(this, &ACharacterStage::OnClicked);
	}

}

// Called every frame
void ACharacterStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterStage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterStage::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{

}


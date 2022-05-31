// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGCharacterBase.h"

// Sets default values
AMMOARPGCharacterBase::AMMOARPGCharacterBase()
	: bFight(false)
	// 	,ID(INDEX_NONE)
	// 	,UserID(INDEX_NONE)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMMOARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMMOARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMMOARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

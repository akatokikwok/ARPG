// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStage.h"
#include "Components/CapsuleComponent.h"
#include "../HallPlayerController.h"

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
		GetCapsuleComponent()->OnClicked.AddDynamic(this, &ACharacterStage::OnClicked_callback);
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

/** 点击3D舞台人物胶囊体后执行的逻辑. */
void ACharacterStage::OnClicked_callback(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (AHallPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AHallPlayerController>()) {
		InPlayerController->ResetTarget(this);// 先重设目标.
		InPlayerController->ExecutionRotateCharacter();// 启用旋转.
	}
}


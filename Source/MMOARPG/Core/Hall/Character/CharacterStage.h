// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStage.generated.h"

/**
 * 点击加号时候被生成的人物.
 */
UCLASS()
class MMOARPG_API ACharacterStage : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//
	UFUNCTION()
		void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

};

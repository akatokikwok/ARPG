// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../DataTable/CharacterAnimTable.h"
#include "MMOARPGCharacterBase.generated.h"

UCLASS()
class MMOARPG_API AMMOARPGCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	friend class AMMOARPGGameMode;
public:
	// Sets default values for this character's properties
	AMMOARPGCharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE bool IsFight() { return bFight; }
	FORCEINLINE FCharacterAnimTable* GetAnimTable() { return AnimTable; }

protected:
	// 是否启用战斗姿势.
	UPROPERTY()
		bool bFight;
	// 用户去配置的ID.
	UPROPERTY(EditDefaultsOnly, Category = "Character")
		int32 ID;
	// 动画表.
		FCharacterAnimTable* AnimTable;
};

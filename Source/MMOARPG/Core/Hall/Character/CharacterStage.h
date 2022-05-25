// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Common/Interface/KneadingInterface.h"
#include "CharacterStage.generated.h"

/**
 * 点击加号时候被生成的人物.
 */
UCLASS()
class MMOARPG_API ACharacterStage : public ACharacter, public IKneadingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterStage();

	// 使用CA存档数据更新外观.
	virtual	void UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA) override;
	// 更新外观.
	virtual	void UpdateKneadingBoby() override;

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
		void OnClicked_callback(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	// 把槽号注册进舞台人物.
	void SetSlotID(int32 InID);
	// 拿取舞台人物的槽号.
	int32 GetSlotID() { return SlotID; }

private:
	int32 SlotID;// 槽号.
};

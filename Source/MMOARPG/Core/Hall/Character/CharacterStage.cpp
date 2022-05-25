// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStage.h"
#include "Components/CapsuleComponent.h"
#include "../HallPlayerController.h"
#include "../HallPlayerState.h"

// Sets default values
ACharacterStage::ACharacterStage()
	: SlotID(INDEX_NONE)// 槽号默认给-1.
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterStage::UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);

	// 需要Mesh也要和我们的骨骼保持一致变化.
	SetMeshPostion(GetMesh());
}

void ACharacterStage::UpdateKneadingBoby()
{
	if (SlotID != INDEX_NONE) {// 异常保护.

		if (AHallPlayerState* InState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>()) {
			if (FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotID)) {// 槽号的设定在 UUI_CharacterCreatePanel::SpawnCharacter里完成.
				UpdateKneadingBoby(*InCA);// 生成指定CA存档的人物身材.
			}
		}
	}
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

void ACharacterStage::SetSlotID(int32 InID)
{
	SlotID = InID;
}


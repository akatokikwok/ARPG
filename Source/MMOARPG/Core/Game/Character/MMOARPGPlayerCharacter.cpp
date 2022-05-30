// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOARPGPlayerCharacter.h"


void AMMOARPGPlayerCharacter::UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);

	//需要Mesh也要和我们的骨骼一致变化
	SetMeshPostion(GetMesh());
}

void AMMOARPGPlayerCharacter::UpdateKneadingBoby()
{
	
}

void AMMOARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetComponentLocation());
}

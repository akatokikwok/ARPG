﻿// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify/AnimNotify_StopAnimMontage.h"
#include "GameFramework/Character.h"

UAnimNotify_StopAnimMontage::UAnimNotify_StopAnimMontage()
{

}

FString UAnimNotify_StopAnimMontage::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_StopAnimMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		InCharacter->StopAnimMontage();
	}
}
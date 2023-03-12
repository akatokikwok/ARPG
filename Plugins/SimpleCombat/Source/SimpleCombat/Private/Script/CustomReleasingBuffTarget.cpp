// Fill out your copyright notice in the Description page of Project Settings.
#include "Script/CustomReleasingBuffTarget.h"
#include "Kismet/GameplayStatics.h"

void UCustomReleasingBuffTarget::GetCustomReleasingBuffTarget(AActor * InOwner, TArray<AActor*>&OutTarget)
{
	K2_GetCustomReleasingBuffTarget(InOwner, OutTarget);
}

UWorld* UCustomReleasingBuffTarget::GetWorld_BuffTarget(AActor* InOwner)
{
	return InOwner->GetWorld();
}

bool UCustomReleasingBuffTarget::GetAllActorsOfClass(AActor* InOwner, TSubclassOf<AActor> ActorClass, TArray<AActor*>& InArray)
{
	UGameplayStatics::GetAllActorsOfClass(InOwner->GetWorld(), ActorClass, InArray);

	return InArray.Num()> 0;
}

// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGPlayerController.h"
#include "Character/MMOARPGCharacter.h"
#include "Character/MMOARPGPlayerCharacter.h"
#include "MMOARPGGameState.h"
#include "MMOARPGPlayerState.h"

AMMOARPGPlayerController::AMMOARPGPlayerController()
{
	bShowMouseCursor = true;
}

void AMMOARPGPlayerController::ReplaceCharacter_Implementation(int32 InCharacterID)
{
// 	if (!GetPawn())
// 	{
// 		return;
// 	}
// 
// 	if (AMMOARPGCharacterBase* MMOARPGBase = GetPawn<AMMOARPGCharacterBase>())
// 	{
// 		if (MMOARPGBase->GetID() == InCharacterID)
// 		{
// 			return;
// 		}
// 	}
// 
// 	if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>())
// 	{
// 		if (FCharacterStyleTable* InStyleTable = InGameState->GetCharacterStyleTable(InCharacterID))
// 		{
// 			if (AMMOARPGCharacter* InNewCharacter = GetWorld()->SpawnActor<AMMOARPGCharacter>(
// 				InStyleTable->MMOARPGCharacterClass,
// 				GetPawn()->GetActorLocation(),
// 				GetPawn()->GetActorRotation()))
// 			{
// 				if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>())
// 				{
// 					//判断是不是主要玩家角色
// 					if (AMMOARPGPlayerCharacter* InPlayerCharacter = Cast<AMMOARPGPlayerCharacter>(InNewCharacter))
// 					{
// 						InPlayerCharacter->UpdateKneadingBoby(InPlayerState->GetCA());
// 						InPlayerCharacter->CallUpdateKneadingBobyOnClient(InPlayerState->GetCA());
// 					}
// 
// 					APawn* InPawn = GetPawn();
// 					OnPossess(InNewCharacter);
// 
// 					InPawn->Destroy(true);
// 				}
// 			}
// 		}
// 	}
}

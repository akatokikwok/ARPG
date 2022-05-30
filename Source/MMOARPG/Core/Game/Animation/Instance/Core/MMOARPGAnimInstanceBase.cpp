// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOARPGAnimInstanceBase.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
// #include "SimpleAdvancedAnimationBPLibrary.h"

// UMMOARPGAnimInstanceBase::UMMOARPGAnimInstanceBase()
// 	:bDeath(false)
// 	,Speed(0.f)
// 	, bInAir(false)
// 	, bFight(false)
// 	, bFootIK(false)
// 	, LeftBoneName(TEXT("foot_l"))
// 	, RightBoneName(TEXT("foot_r"))
// 	, FookIKID(INDEX_NONE)
// {
// 
// }

// void UMMOARPGAnimInstanceBase::InitAnimInstance(ACharacter* InCharacter)
// {
// 	if (bFootIK)
// 	{
// 		BoneNames.Add(LeftBoneName);
// 		BoneNames.Add(RightBoneName);
// 
// 		FookIKID = USimpleAdvancedAnimationBPLibrary::CreateFootIK(InCharacter, BoneNames);
// 	}
// }
// 
// void UMMOARPGAnimInstanceBase::NativeInitializeAnimation()
// {
// 	Super::NativeInitializeAnimation();
// }
// 
// void UMMOARPGAnimInstanceBase::NativeUpdateAnimation(float Deltaseconds)
// {
// 	Super::NativeUpdateAnimation(Deltaseconds);
// 
// 	if (AMMOARPGCharacterBase *InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
// 	{
// 		Speed = InCharacterBase->GetVelocity().Size();
// 		bInAir = InCharacterBase->GetMovementComponent()->IsFalling();
// 		bFight = InCharacterBase->IsFight();
// 	}
// 
// 	if (bFootIK && FookIKID != INDEX_NONE)
// 	{
// 		TArray<float> OffsetArray;
// 	
// 		//考虑双腿
// 		float LOffset = GetFootIKOffset(LeftBoneName);
// 		float ROffset = GetFootIKOffset(RightBoneName);
// 
// 		OffsetArray.Add(LOffset);
// 		OffsetArray.Add(ROffset);
// 
// 		ButtZOffset = USimpleAdvancedAnimationBPLibrary::GetButtZOffset(OffsetArray);
// 
// 		LeftOffset = -(ButtZOffset - LOffset);
// 		RightOffset = ButtZOffset - ROffset;
// 	}
// }
// 
// float UMMOARPGAnimInstanceBase::GetFootIKOffset(const FName& InBoneName)
// {
// 	if (FookIKID != INDEX_NONE)
// 	{
// 		return USimpleAdvancedAnimationBPLibrary::FindOffset(FookIKID,InBoneName);
// 	}
// 
// 	return 0.0f;
// }

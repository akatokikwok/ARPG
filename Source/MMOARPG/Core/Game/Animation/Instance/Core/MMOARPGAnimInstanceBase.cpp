// Fill out your copyright notice in the Description page of Project Settings.

#include "MMOARPGAnimInstanceBase.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SimpleAdvancedAnimationBPLibrary.h"

UMMOARPGAnimInstanceBase::UMMOARPGAnimInstanceBase()
	: bDeath(false)
	, DieIndex(0)
	, Speed(0.0f)
	, bInAir(false)
	, ActionState(ECharacterActionState::NORMAL_STATE)
	, FookIKID(INDEX_NONE)
	, bFootIK(false)
	, LeftBoneName(TEXT("foot_l"))
	, RightBoneName(TEXT("foot_r"))
{

}

void UMMOARPGAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMOARPGAnimInstanceBase::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner())) {// 拿到装载本AnimIns的人物类.
		Speed = InCharacterBase->GetVelocity().Size();// 拿人的速率.
		bInAir = InCharacterBase->GetMovementComponent()->IsFalling();// 空中检测.
		ActionState = InCharacterBase->GetActionState();// 战斗姿态启用检测.
		bDeath = InCharacterBase->IsDie();// 人是否死亡.
		DieIndex = InCharacterBase->GetDieIndex();// 死亡动画序列号.

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent())) {
			/* 仅在攀岩模式下. */
			if (InCharacterMovementComponent->MovementMode != EMovementMode::MOVE_Custom) {// 必须是非攀岩模式下才允许IK
				if (bFootIK && FookIKID != INDEX_NONE) {
					TArray<float> OffsetArray;

					// 仅考虑双腿
					float LOffsetVal = this->GetFootIKOffset(LeftBoneName);
					float ROffsetVal = this->GetFootIKOffset(RightBoneName);

					OffsetArray.Add(LOffsetVal);
					OffsetArray.Add(ROffsetVal);

					ButtZOffset = USimpleAdvancedAnimationBPLibrary::GetButtZOffset(OffsetArray);

					LeftOffset = (-1) * (ButtZOffset - LOffsetVal);
					RightOffset = (1) * ButtZOffset - ROffsetVal;
				}
			}
		}


		
	}
}

float UMMOARPGAnimInstanceBase::GetFootIKOffset(const FName& InBoneName)
{
	if (FookIKID != INDEX_NONE) {
		return USimpleAdvancedAnimationBPLibrary::FindOffset(FookIKID, InBoneName);
	}

	return 0.0f;
}

/** 初始化动画实例,构建一些数据,例如IK; 仅当启用IK时, 允许为一系列骨骼构建 IK数据. */
void UMMOARPGAnimInstanceBase::InitAnimInstance(ACharacter* InCharacter)
{
	if (bFootIK == true) {
		BoneNames.Add(LeftBoneName);
		BoneNames.Add(RightBoneName);

		FookIKID = USimpleAdvancedAnimationBPLibrary::CreateFootIK(InCharacter, BoneNames, mFootDrawLineTraceType);


	}
}

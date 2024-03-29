﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState/AnimNotifyState_CheckCombo.h"
#include "GameFramework/Character.h"
#include "CombatInterface/SimpleCombatInterface.h"

/** 在蒙太奇里Begin时, 招式段号递增至下一段并视作是长按. */
void UAnimNotifyState_CheckCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	/** 仅在服务器上执行连招计数, 而GetWorld()->IsServer()则强制定位至服务器.
	 * 阻止客户端也在修改攻击的Index,仅承认服务器的.	
	 */
	if (MeshComp->GetWorld() && MeshComp->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
		if (ISimpleComboInterface* InCharacter = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
			InCharacter->GetSimpleComboInfo(ComboKey_GAName)->bShortPress = false;
			InCharacter->GetSimpleComboInfo(ComboKey_GAName)->UpdateComboIndex();// 递增更新招式段号.
		}
	}
}

void UAnimNotifyState_CheckCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

}

/** 在蒙太奇里End时, 触发GA平砍. */
void UAnimNotifyState_CheckCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	/** 仅在服务器上执行连招计数, 而GetWorld()->IsServer()则强制定位至服务器. */
	if (MeshComp->GetWorld() && MeshComp->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
		// 无论短按还是长按,只要有一个成功就认可
		if (ISimpleComboInterface* InCharacter = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
			if (InCharacter->GetSimpleComboInfo(ComboKey_GAName)->bLongPress || InCharacter->GetSimpleComboInfo(ComboKey_GAName)->bShortPress) {
				InCharacter->ComboAttack(ComboKey_GAName);
			}
		}
	}
}

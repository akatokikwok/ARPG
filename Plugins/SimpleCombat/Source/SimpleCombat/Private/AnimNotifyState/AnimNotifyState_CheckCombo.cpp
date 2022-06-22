// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState/AnimNotifyState_CheckCombo.h"
#include "GameFramework/Character.h"
#include "CombatInterface/SimpleCombatInterface.h"

/** 在蒙太奇里Begin时, 招式段号递增至下一段并视作是长按. */
void UAnimNotifyState_CheckCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (ISimpleComboInterface* InCharacter = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
		InCharacter->GetSimpleComboInfo()->bShortPress = false;
		InCharacter->GetSimpleComboInfo()->UpdateComboIndex();// 递增更新招式段号.
	}
}

void UAnimNotifyState_CheckCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

/** 在蒙太奇里End时, 触发GA平砍. */
void UAnimNotifyState_CheckCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	// 无论短按还是长按,只要有一个成功就认可
	if (ISimpleComboInterface* InCharacter = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
		if (InCharacter->GetSimpleComboInfo()->bLongPress || InCharacter->GetSimpleComboInfo()->bShortPress) {
			InCharacter->ComboAttack(ComboKey_GAName);
		}
	}
}

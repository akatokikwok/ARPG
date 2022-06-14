// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState/AnimNotifyState_CheckCombo.h"
#include "GameFramework/Character.h"
#include "CombatInterface/SimpleCombatInterface.h"

/**  */
void UAnimNotifyState_CheckCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (ISimpleComboInterface* InCharacter = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
		InCharacter->GetSimpleComboInfo()->bShortPress = false;
		InCharacter->GetSimpleComboInfo()->UpdateComboIndex();// 持续循环section号
	}
}

void UAnimNotifyState_CheckCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

/**  */
void UAnimNotifyState_CheckCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	// 无论短按还是长按,只要有一个成功就认可
	if (ISimpleComboInterface* InCharacter = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
		if (InCharacter->GetSimpleComboInfo()->bLongPress || InCharacter->GetSimpleComboInfo()->bShortPress) {
			InCharacter->ComboAttack(ComboKey);// 放出连击技能.
		}
	}
}

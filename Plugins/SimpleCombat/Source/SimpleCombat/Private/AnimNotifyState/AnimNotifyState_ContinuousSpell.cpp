// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState/AnimNotifyState_ContinuousSpell.h"
#include "SimpleComboType.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "GameFramework/Character.h"

void UAnimNotifyState_ContinuousSpell::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp->GetOuter() && MeshComp->GetOuter()->GetWorld()) {
		if (ISimpleComboInterface* InCharacter = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {// 人必须继承了战斗接口
			if (FContinuousReleaseSpell* InSpell = InCharacter->GetContinuousReleaseSpell()) {// 取得持续施法黑盒
				if (InSpell->ContinuousReleaseSpellIndex == 0 || InSpell->ContinuousReleaseSpellIndex == INDEX_NONE) {
					// 进入下一个循环或者刚开始,这两种情形, 会设定一下持续施法号为 section 1
					InSpell->ContinuousReleaseSpellIndex = 1;
				}
			}
		}
	}
}

void UAnimNotifyState_ContinuousSpell::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_ContinuousSpell::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp->GetOuter() && MeshComp->GetOuter()->GetWorld()) {
		if (ISimpleComboInterface* InCharacterInterface = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
			if (FContinuousReleaseSpell* InSpell = InCharacterInterface->GetContinuousReleaseSpell()) {
				if (InSpell->AnimMontage) {
					FName SectionName = *FString::FromInt(InSpell->ContinuousReleaseSpellIndex);
					if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
						// 计数为2的时候, 即可以视作是中断持续施法, 具体行为是播放 2号section的动画(End)
						if (InSpell->ContinuousReleaseSpellIndex == 2) {
							InCharacter->PlayAnimMontage(InSpell->AnimMontage, 1.f,	SectionName);
						}
						// 核验计数为1或者0, 即可以视作是保持持续施法或者是前摇动作, 具体行为是跳转过去1 并Loop
						else {
							if (InCharacter->GetMesh()) {
								if (UAnimInstance* InAnimInstance = InCharacter->GetMesh()->GetAnimInstance()) {
									InAnimInstance->Montage_JumpToSection(SectionName);
									InAnimInstance->Montage_SetNextSection(SectionName, SectionName);
								}
							}
						}
					}
				}
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.
#include "ExecutionCalculation_PlayMontage.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "AbilitySystemComponent.h"

UExecutionCalculation_PlayMontage::UExecutionCalculation_PlayMontage()
	: bCohesiveAnimation(true)
	, PlayAnimPos(0.f)
	, PlayRate(1.f)
	, bStopAllAnimMontage(true)
	, StartSectionName(NAME_None)
{

}

void UExecutionCalculation_PlayMontage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetAC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetAC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(SourceAC->GetOwner())) {
			if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
				// 待加工的1个蒙太奇
				UAnimMontage* InMontage_Process = NULL;

				if (bCohesiveAnimation) {/** 启用衔接开关(那些有施法前摇或者后摇的情况) */
					if (UAnimInstance* InAnimInstance = SourceAC->AbilityActorInfo->GetAnimInstance()) {// 先拿到自己ASC下管控的动画实例
						// 取得当前ASC控制下的蒙太奇
						InMontage_Process = SourceAC->GetCurrentMontage();
						
						/* 蒙太奇已播放的进度 未达到 预设进度, 则不做处理*/
						if (InMontage_Process) {
							float CurrentAnimPos = InAnimInstance->Montage_GetPosition(InMontage_Process);// 实例内蒙太奇已播放的进度
							if (CurrentAnimPos < PlayAnimPos) {// 已播放进度 未达到 预设的进度
								InMontage_Process = InMontage_Process;
							}
						}
					}
				}
				else {/** 不启用衔接 */
					InMontage_Process = MontageToPlay;// 蒙太奇替换为 不启用衔接直接暴力替换的新蒙太奇
				}

				if (InMontage_Process != nullptr) {
					// 让自己播放指定蒙太奇的指定Section的指定位置
					InCharacter->MontagePlayOnMulticast(
						InMontage_Process,
						PlayRate,
						PlayAnimPos,
						bStopAllAnimMontage,
						StartSectionName);

					InCharacter->AnimSignal(2);// 复位连打
				}
			}
		}
	}
}
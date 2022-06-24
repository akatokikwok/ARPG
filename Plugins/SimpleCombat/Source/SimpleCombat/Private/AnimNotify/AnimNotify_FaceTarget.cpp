// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify/AnimNotify_FaceTarget.h"
#include "CombatInterface/SimpleCombatInterface.h"

UAnimNotify_FaceTarget::UAnimNotify_FaceTarget()
{
	bYaw = true;
	bPitch = false;
	bRoll = false;
}

FString UAnimNotify_FaceTarget::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_FaceTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (ISimpleComboInterface* InSimpleCombatInterface = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
		if (AActor* InTarget = InSimpleCombatInterface->GetTarget()) {// 先拿敌对目标.
			if (AActor* InThis = Cast<AActor>(MeshComp->GetOuter())) {// 再拿自己.

				FRotator FaceRot = FRotationMatrix::MakeFromX(InTarget->GetActorLocation() - InThis->GetActorLocation()).Rotator();// 施法者重转向受击者的弧度
				
				auto ResetValue = [](bool bReset, double& InAix) {
					if (!bReset) {
						InAix = 0.f;
					}
				};
				ResetValue(bYaw, FaceRot.Yaw);
				ResetValue(bRoll, FaceRot.Roll);
				ResetValue(bPitch, FaceRot.Pitch);

				InThis->SetActorRotation(FaceRot);
			}
		}
	}
}
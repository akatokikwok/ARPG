#include "AnimNotifyState/AnimNotifyState_BlockInputInTime.h"
#include "GameFramework/Character.h"

void UAnimNotifyState_BlockInputInTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		if (InCharacter->GetController()) {
			InCharacter->GetController()->SetIgnoreMoveInput(true);// 忽略移动输入.
		}
	}
}

void UAnimNotifyState_BlockInputInTime::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_BlockInputInTime::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		if (InCharacter->GetController()) {
			InCharacter->GetController()->SetIgnoreMoveInput(false);
		}
	}
}
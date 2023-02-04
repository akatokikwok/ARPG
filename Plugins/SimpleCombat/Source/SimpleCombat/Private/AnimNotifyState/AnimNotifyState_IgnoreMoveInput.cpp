#include "AnimNotifyState/AnimNotifyState_IgnoreMoveInput.h"
#include "GameFramework/Character.h"

void UAnimNotifyState_IgnoreMoveInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		// 仅在客户端mode, 主机玩家下:
		if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Client)) {
			if (InCharacter->GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
				InCharacter->GetController()->SetIgnoreMoveInput(true);
			}
		}
		// 在standalone mode 或者监听服务器下.
		else if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Standalone) || InCharacter->GetWorld()->IsNetMode(ENetMode::NM_ListenServer)) {
			InCharacter->GetController()->SetIgnoreMoveInput(true);
		}
	}
}

void UAnimNotifyState_IgnoreMoveInput::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_IgnoreMoveInput::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {

		// 仅在客户端mode, 主机玩家下:
		if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Client)) {
			if (InCharacter->GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
				InCharacter->GetController()->SetIgnoreMoveInput(false);
			}
		}
		// 在standalone mode 或者监听服务器下.
		else if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Standalone) || InCharacter->GetWorld()->IsNetMode(ENetMode::NM_ListenServer)) {
			InCharacter->GetController()->SetIgnoreMoveInput(false);
		}
	}
}
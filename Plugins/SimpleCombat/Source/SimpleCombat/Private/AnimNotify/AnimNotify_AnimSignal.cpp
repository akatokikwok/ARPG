#include "AnimNotify/AnimNotify_AnimSignal.h"
#include "CombatInterface/SimpleCombatInterface.h"

UAnimNotify_AnimSignal::UAnimNotify_AnimSignal()
	: SignalValue(INDEX_NONE)
{

}

FString UAnimNotify_AnimSignal::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_AnimSignal::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	// 检测人是不是携带了Combat UInterface,携带了就执行信号,
	if (ISimpleComboInterface* InSimpleCombatInterface = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {
		InSimpleCombatInterface->AnimSignal(SignalValue);
	}
}

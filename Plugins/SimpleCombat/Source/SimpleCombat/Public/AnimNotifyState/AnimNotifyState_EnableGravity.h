#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_EnableGravity.generated.h"

/**
 * NS, 控制重力
 */
UCLASS(meta = (DisplayName = "EnableGravity"))
class SIMPLECOMBAT_API UAnimNotifyState_EnableGravity : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_EnableGravity() {}

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
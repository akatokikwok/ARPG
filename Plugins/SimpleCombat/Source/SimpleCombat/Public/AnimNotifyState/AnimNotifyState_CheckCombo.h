// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CheckCombo.generated.h"

/**
 * 专门负责执行连击效果的AnimNotifyState.
 */
UCLASS(meta = (DisplayName = "CheckCombo"))
class SIMPLECOMBAT_API UAnimNotifyState_CheckCombo : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	/** 招式段号递增至下一段并视作是长按. */
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	/**  */
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	/** 在蒙太奇里End时, 触发GA平砍.  */
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	// 明确的技能名. 在蒙太奇蓝图资源里为本NotifyState手动配置其名称.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AnimNotifyState_CheckCombo")
		FName ComboKey_GAName;

};

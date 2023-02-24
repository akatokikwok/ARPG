// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AnimSignal.generated.h"

/**
 * 借助信号量来触发特定逻辑的 Notify.
 */
UCLASS(meta = (DisplayName = "AnimSignal"))// 让类名UAnimNotify_AnimSignal在蓝图资源里显示为"AnimSignal".
class SIMPLECOMBAT_API UAnimNotify_AnimSignal : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_AnimSignal();

	// 重写GetNotifyName_Implementation.
	virtual FString GetNotifyName_Implementation() const override;
	// 重写Notify.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	/**
	 * 信号值,可以被设为不同的浮点数,挂载在AnimSeq或者蒙太奇上的Notify槽位追踪里
	 * 0.f, 1.f 设定为装卸武器
	 * 2.f 设定为复位Combo触发器.
	 * 7 和 8 设定为播 死亡蒙太奇的哪个section.
	 * 9 10 表示激活挑飞 和禁用挑飞(落地)
	 * 11 是在异常状态结束后人物起身
	 * 15 是振刀启用
	 * 16 是振刀结束
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		int32 SignalValue;
	
};

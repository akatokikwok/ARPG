// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_Static_Effects.generated.h"

class UParticleSystem;
class UNiagaraSystem;
class AHitCollision;

/**
 * 静态Cue, 负责生成奶瓜/或者传统粒子, 有多项参数蓝图可配.
 */
UCLASS()
class MMOARPG_API UGameplayCueNotify_Static_Effects : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UGameplayCueNotify_Static_Effects();
	// 覆写应用Cue的接口
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters);

public:
	// 粒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		UParticleSystem* EmitterTemplate;

	// 奶瓜
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		UNiagaraSystem* NiagaraTemplate;

	// 需要指定的特效里的那个对外暴露的参数名字.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		FName DurationName;

	// 是否开启附着
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		bool bAttach;

	// 附着点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA", meta = (EditCondition = "bAttach"))
		FName AttachPointName;

	// 附着类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA", meta = (EditCondition = "bAttach"))
		TEnumAsByte<EAttachLocation::Type> AttachLocation;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		FVector PositionOffset;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		FRotator RotatorOffset;

	// 碰撞盒子tsub
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		TSubclassOf<AHitCollision> HitCollisionClass;

	// 碰撞位置偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		FVector CollisionPositionOffset;

	// 碰撞旋转角度偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA")
		FRotator CollisionRotatorOffset;
};
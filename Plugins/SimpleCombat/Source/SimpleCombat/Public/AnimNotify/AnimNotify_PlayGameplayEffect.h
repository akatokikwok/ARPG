// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayGameplayEffect.generated.h"

class UGameplayEffect;
class UCustomReleasingBuffTarget;

/**
 * 动画通知: 负责施加主动buff应用至1个对象.
 */
UCLASS(meta = (DisplayName = "PlayGameplayEffect"))
class SIMPLECOMBAT_API UAnimNotify_PlayGameplayEffect : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_PlayGameplayEffect();
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	// 勾选应用给他人, 反选应用给自己
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		bool bTarget;

	// 欲应用的GE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		TSubclassOf<UGameplayEffect> EffectClass;

	// 获取策划限定的特殊目标的定制脚本;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		TSubclassOf<UCustomReleasingBuffTarget> CustomReleasingBuffTarget;
};

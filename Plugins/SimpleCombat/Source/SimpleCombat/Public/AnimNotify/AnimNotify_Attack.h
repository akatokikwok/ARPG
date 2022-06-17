// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Attack.generated.h"
class AHitCollision;


/**
 * Notify点状态
 * 用于攻击和受击生成actor碰撞等战斗逻辑.
 */
UCLASS(meta = (DisplayName = "Attack"))// 让类名UAnimNotify_Attack在蓝图资源里显示为"Attack".
class SIMPLECOMBAT_API UAnimNotify_Attack : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_Attack();

	// 重写GetNotifyName_Implementation.
	virtual FString GetNotifyName_Implementation() const override;
	// 重写Notify.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

#if WITH_EDITORONLY_DATA
	virtual FLinearColor GetEditorColor() override;
#endif

public:
	// 要生成什么物体?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		TSubclassOf<AHitCollision> HitObjectClass;// 要生成什么物体?

	// 碰撞体的相对位置.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		FVector RelativeLocation;// 碰撞体的相对位置.

	// 存在的生命周期.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		float LifeTime;// 存在的生命周期.

	// 绑在哪个骨骼socket上.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		FName InSocketName;// 绑在哪个骨骼socket上.

	// 是否允许将Box绑定在角色身上开火点上.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		bool bBind;// 是否允许将Box绑定在角色身上开火点上.

	// 群伤还是单体伤害.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		bool bSingleTarget;// 群伤还是单体伤害.
};

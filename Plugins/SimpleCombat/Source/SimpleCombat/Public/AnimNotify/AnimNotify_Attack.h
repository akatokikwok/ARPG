// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
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
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITORONLY_DATA
	// 覆写此notify在编辑器蒙太奇中的显示颜色.
	virtual FLinearColor GetEditorColor() override;
#endif

#if WITH_EDITOR
	// 覆写自UObject的虚方法; 它负责在编辑器里修改字段后重新影响刷新生效.
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 要生成什么物体?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		TSubclassOf<AHitCollision> HitObjectClass;// 要生成什么物体?
	
	// 被你攻击的受击者beHit蒙太奇里的指定受击ID.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		int32 HitID;// 被你攻击的受击者beHit蒙太奇里的指定受击ID..

	// 是否启用GameplayTag而不是FName型来判断buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		bool bEnableBuffTag;

	// 技能带来的一组buff名称(FName)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		TArray<FName> Buffs;

	// 技能带来的一组buff名称(GameplayTag)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration", meta = (EditCondition = "bEnableBuffTag"))
		TArray<FGameplayTag> BuffTags;

	// 是否生成并渲染hitbox; 如果为true 代表仅服务器生成 如果为false 则在双端生成
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		bool bSpawnCollisionOnServer;// 是否生成并渲染hitbox; 如果为true 代表仅服务器生成 如果为false 则在双端生成..

	// 碰撞体再补上一段偏移位置.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		FVector RelativeOffsetLocation;// 碰撞体再补上一段偏移位置.

	// 碰撞体的旋转偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		FRotator RotationOffset;

	// 存在的生命周期.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		float LifeTime;// 存在的生命周期.

	// 绑在哪个骨骼socket上.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		FName InSocketName;// 绑在哪个骨骼socket上.

	// 是否允许将Box绑定在角色身上开火点(实际上是手动添加的一个socket)上.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		bool bBind;// 是否允许将Box绑定在角色身上开火点(实际上是手动添加的一个socket)上..

	// 群伤还是单体伤害.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General configuration")
		bool bSingleTarget;// 群伤还是单体伤害.

public:
	// 是否是盒子hitbox;
	UPROPERTY(BlueprintReadOnly)
		bool bHitBox;// 是否是盒子hitbox;
	
	// 是否是球体hitbox;
	UPROPERTY(BlueprintReadOnly)
		bool bHitSphere;// 是否是球体hitbox;
	
	// 是否是自定义体hitbox
	UPROPERTY(BlueprintReadOnly)
		bool bHitCustom;// 是否是自定义体hitbox;
	
	// 是否是胶囊体hitbox
	UPROPERTY(BlueprintReadOnly)
		bool bHitCapsule;// 是否是胶囊体hitbox;
public:
	// 用以控制盒形碰撞的参数; meta = (EditCondition = "bHitBox")意为字段bHitBox开启后才允许编辑本字段.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Box", meta = (EditCondition = "bHitBox"))
		FVector BoxExtent;
	// 用以控制球形碰撞的参数; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Sphere", meta = (EditCondition = "bHitSphere"))
		float SphereRadius;
	// 用以控制自定义形碰撞的参数; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Capsule", meta = (EditCondition = "bHitCapsule"))
		float CapsuleHalfHeight;
	// 用以控制胶囊形碰撞的参数; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Capsule", meta = (EditCondition = "bHitCapsule"))
		float CapsuleRadius;
};

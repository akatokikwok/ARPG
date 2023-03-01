#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SimpleComboType.h"
#include "ComboSkillHitCollision.generated.h"

class ACharacter;

/**
 * 一切hitbox的基类.
 * 生成的碰撞actor,用于普攻攻击时候检测.
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API AHitCollision : public AActor
{
	GENERATED_BODY()
public:
	//
	AHitCollision(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 回调虚方法: 处理碰撞接触时候的逻辑;  可由子类覆写.
	UFUNCTION()
		virtual void HandleDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 虚方法,让派生类覆写; 拿取对应的形状comp;
	virtual UPrimitiveComponent* GetHitDamage();

	// 设置 具备形状comp 的相对位置.
	void SetHitDamageRelativePosition(const FVector& InNewPostion);

	// 操作启用或禁用hitbox碰撞.
	void Collision(bool bTurnOnCollision);

	// 写入受击ID
	void SetHitID(int32 InNewHit) { HitID = InNewHit; }
	// 拿取受击ID
	const int32 GetHitID() const { return HitID; }

	// 设定一组buff名字(FName型)
	void SetBuffs(const TArray<FName>& InBuffNames) { Buffs = InBuffNames; }
	// 设定一组buff名字(FGameplayTag型)
	void SetBuffs(const TArray<FGameplayTag>& InBuffTags) { BuffTags = InBuffTags; }

	// 核验给定的pawn已被攻击过
	bool IsExist(ACharacter* InNewTarget) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCollision", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* HitCollisionRootComponent;

	// 提供初速度的运动弹丸组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCollision", meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

public:
	// 碰撞伤害类型,默认情况下是近程攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitCollision")
		EHitCollisionType HitCollisionType;

	// 开火特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitCollision")
		class UNiagaraSystem* OpenFireNiagara;

	// 开火特效
	UPROPERTY(EditDefaultsOnly, Category = "HitCollision")
		class UParticleSystem* OpenFireParticle;

	// 开火点名称
	UPROPERTY(EditDefaultsOnly, Category = "HitCollision")
		FName OpenFirePoint;

	//
	UPROPERTY(EditDefaultsOnly, Category = "HitCollision")
		bool bRandomDirection;

protected:
	// 受击ID.
	UPROPERTY()
		int32 HitID;// 受击ID.

	// 由技能施加的一组buff名称(FName型).
	UPROPERTY()
		TArray<FName> Buffs;

	// 由技能施加的一组buff名称(Tag型).
	UPROPERTY()
		TArray<FGameplayTag> BuffTags;

	// 已攻击过的目标们
	UPROPERTY()
		TArray<TWeakObjectPtr<ACharacter>> AttackedTarget;

	// 配合技能用的追踪曲线.
	UPROPERTY()
		class USplineComponent* Spline;

	// 持续施法追踪曲线的持续时长
	UPROPERTY()
		float CurrentSplineTime;

	// 敌人到我的向量
	UPROPERTY()
		FVector DistanceVector;
};
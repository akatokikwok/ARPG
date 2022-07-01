#pragma once

#include "CoreMinimal.h"
#include "ComboSkillHitCollision.generated.h"

/**
 * 一切hitbox的基类.
 * 生成的碰撞actor,用于普攻攻击时候检测.
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API AHitCollision : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCollision", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* HitCollisionRootComponent;
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
	// 设定一组buff名字
	void SetBuffs(const TArray<FName>& InBuffs) { Buffs = InBuffs; }

protected:
	// 受击ID.
	UPROPERTY()
		int32 HitID;// 受击ID.
	
	// 由技能施加的一组buff名称.
	UPROPERTY()
		TArray<FName> Buffs;
};
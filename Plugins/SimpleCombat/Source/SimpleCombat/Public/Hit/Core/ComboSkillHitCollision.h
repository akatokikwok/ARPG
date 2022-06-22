﻿#pragma once

#include "CoreMinimal.h"
#include "ComboSkillHitCollision.generated.h"

/**
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
};
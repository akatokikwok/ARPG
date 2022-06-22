#pragma once

#include "CoreMinimal.h"
#include "Core/ComboSkillHitCollision.h"
#include "ComboSkillHitBox.generated.h"

/**
 * 盒子碰撞actor, 派生自基类 AHitCollision
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API AHitBoxCollision : public AHitCollision
{
	GENERATED_BODY()
private:
	// 盒子形状碰撞盒子,它本质上是1个BoxComp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* HitDamage;// 盒子形状碰撞盒子,它本质上是1个BoxComp..
public:
	//
	AHitBoxCollision(const FObjectInitializer& ObjectInitializer);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 覆写基类的. 拿取盒子形Comp.
	virtual UPrimitiveComponent* GetHitDamage() override;
	
	// 影响外观; 设定盒子的extent;
	void SetBoxExtent(const FVector& InNewBoxExtent);
};
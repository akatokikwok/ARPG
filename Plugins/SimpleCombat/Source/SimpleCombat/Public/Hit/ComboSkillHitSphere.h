#pragma once

#include "CoreMinimal.h"
#include "Core/ComboSkillHitCollision.h"
#include "ComboSkillHitSphere.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API AHitSphereCollision : public AHitCollision
{
	GENERATED_BODY()
private:
	// 球体形状碰撞盒子.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* HitDamage;
public:
	//
	AHitSphereCollision(const FObjectInitializer& ObjectInitializer);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 覆写基类的.拿取球形Comp.
	virtual UPrimitiveComponent* GetHitDamage() override;
};
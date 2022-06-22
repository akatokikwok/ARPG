#pragma once

#include "CoreMinimal.h"
#include "Core/ComboSkillHitCollision.h"
#include "ComboSkillHitCapsule.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API AHitCapsuleCollision : public AHitCollision
{
	GENERATED_BODY()
private:
	// 胶囊形状的碰撞盒子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* HitDamage;
public:
	//
	AHitCapsuleCollision(const FObjectInitializer& ObjectInitializer);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 覆写基类的. 拿取胶囊型comp.
	virtual UPrimitiveComponent* GetHitDamage() override;

	// 影响外观; 设定胶囊的半径和半高
	void SetCapsuleHalfHeight(float InNewCapsuleHalfHeight);
	void SetCapsuleRadius(float InNewCapsuleRadius);
};
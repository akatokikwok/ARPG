#pragma once

#include "CoreMinimal.h"
#include "Hit/ComboSkillHitBox.h"
#include "MMOARPGBoxHit.generated.h"

/**
 * BoxHit类,模拟撞击盒子hitbox伤害或者射击子弹
 * 项目里真正使用到的hitbox
 */
UCLASS(BlueprintType, Blueprintable)
class AMMOARPGBoxHit : public AHitBoxCollision
{
	GENERATED_BODY()
public:
	AMMOARPGBoxHit(const FObjectInitializer& ObjectInitializer);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 覆写一个回调函数AHitCollision; 当其他武器触碰hitbox时候会引发伤害.
	virtual void HandleDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
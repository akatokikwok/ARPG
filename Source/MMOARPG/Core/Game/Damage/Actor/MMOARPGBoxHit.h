#pragma once

#include "CoreMinimal.h"
#include "Hit/ComboSkillHitBox.h"
#include "MMOARPGBoxHit.generated.h"

/**
 * BoxHit类,模拟伤害或者射击子弹
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

	// 覆写AHitCollision; 
	virtual void HandleDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
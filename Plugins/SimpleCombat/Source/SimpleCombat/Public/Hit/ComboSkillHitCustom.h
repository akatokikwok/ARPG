#pragma once

#include "CoreMinimal.h"
#include "Core/ComboSkillHitCollision.h"
#include "ComboSkillHitCustom.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API AHitCustomCollision : public AHitCollision
{
	GENERATED_BODY()
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
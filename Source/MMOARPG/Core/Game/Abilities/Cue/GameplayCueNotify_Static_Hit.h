#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_Static_Hit.generated.h"
class UParticleSystem;
class UNiagaraSystem;

/**
 *	Cue:当受击时候的特效, 派生自静态的Cue
 */
UCLASS()
class MMOARPG_API UGameplayCueNotify_Static_Hit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA Hit")
		UParticleSystem* HitEmitterTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GA Hit")
		UNiagaraSystem* HitNiagaraTemplate;

};

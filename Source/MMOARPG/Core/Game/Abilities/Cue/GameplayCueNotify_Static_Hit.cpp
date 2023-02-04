// Fill out your copyright notice in the Description page of Project Settings.
#include "GameplayCueNotify_Static_Hit.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGameplayCueNotify_Static_Hit::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	if (MyTarget && !MyTarget->IsPendingKill() && GetWorld()) {
		switch (EventType) {
			case EGameplayCueEvent::Executed:// 检查Cue是不是被执行了
			{
				if (HitEmitterTemplate) {
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						HitEmitterTemplate, MyTarget->GetActorTransform());
				}
				else if (HitNiagaraTemplate) {
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						HitNiagaraTemplate, MyTarget->GetActorLocation());
				}
				break;
			}
		}
	}

	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
}
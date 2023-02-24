// Fill out your copyright notice in the Description page of Project Settings.
#include "GameplayCueNotify_Static_Effects.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"

UGameplayCueNotify_Static_Effects::UGameplayCueNotify_Static_Effects()
	:bAttach(false)
	, AttachLocation(EAttachLocation::KeepWorldPosition)
{

}

void UGameplayCueNotify_Static_Effects::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	if (DurationName.IsValid() && MyTarget && IsValid(MyTarget) && GetWorld()) {
		switch (EventType) {
			case EGameplayCueEvent::OnActive:
			case EGameplayCueEvent::Executed:
			{
				if (EmitterTemplate) {
					UParticleSystemComponent* InParticleSystemComponent = NULL;
					if (bAttach) {
						InParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(
							EmitterTemplate,
							MyTarget->GetRootComponent(),
							AttachPointName,
							MyTarget->GetActorLocation() + MyTarget->GetActorForwardVector() * PositionOffset,
							MyTarget->GetActorRotation() + RotatorOffset, //
							MyTarget->GetActorScale(),
							AttachLocation);
					}
					else {
						FTransform Transform = MyTarget->GetActorTransform();

						Transform.SetLocation(Transform.GetLocation() + MyTarget->GetActorForwardVector() * PositionOffset);
						Transform.SetRotation((Transform.GetRotation().Rotator() + RotatorOffset).Quaternion());

						InParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
							GetWorld(),
							EmitterTemplate, Transform);
					}

					if (InParticleSystemComponent) {
						InParticleSystemComponent->SetFloatParameter(
							DurationName,
							Parameters.RawMagnitude);
					}
				}
				else if (NiagaraTemplate) {
					UNiagaraComponent* InNiagaraComponent = NULL;
					if (bAttach) {
						InNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
							NiagaraTemplate,
							MyTarget->GetRootComponent(),
							AttachPointName,
							MyTarget->GetActorLocation() + MyTarget->GetActorForwardVector() * PositionOffset,
							MyTarget->GetActorRotation() + RotatorOffset,
							AttachLocation,
							true);
					}
					else {
						InNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
							GetWorld(),
							NiagaraTemplate,
							MyTarget->GetActorLocation() + MyTarget->GetActorForwardVector() * PositionOffset,
							MyTarget->GetActorRotation() + RotatorOffset);
					}

					if (InNiagaraComponent) {
						InNiagaraComponent->SetFloatParameter(
							DurationName,
							Parameters.RawMagnitude);
					}
				}
				break;
			}
		}
	}

	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
}
#include "ExecutionCalculation_ZeroVelocity.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UExecutionCalculation_ZeroVelocity::UExecutionCalculation_ZeroVelocity()
{

}

void UExecutionCalculation_ZeroVelocity::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetAC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetAC) {
		if (AMMOARPGCharacterBase* InCharacter = Cast<AMMOARPGCharacterBase>(SourceAC->GetOwner())) {
			if (InCharacter->GetCharacterMovement()) {
				InCharacter->GetCharacterMovement()->StopMovementImmediately();
			}
		}
	}
}
#pragma once

#include "CoreMinimal.h"
#include "MMOARPGGameplayAbilityType.generated.h"

/**
 * 明确指明由某个GA诱发的某些buff(GE).
 */
USTRUCT(BlueprintType)
struct FMMOARPGGameplayEffects
{
	GENERATED_BODY()

public:
	// GE集
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		TArray<TSubclassOf<UGameplayEffect>> TargetEffectClasses;// GE群集
};

/**
 * 明确指明由某个GA诱发的 GE包
 * GE包内携带GE作用目标数据、 GE-Handle
 */
USTRUCT(BlueprintType)
struct FMMOARPGGameplayEffectSpec
{
	GENERATED_BODY()

public:
	// GE作用的数据集
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		FGameplayAbilityTargetDataHandle TargetHandleData;// GE作用的数据集
	// 一组真正的GE句柄.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		TArray<FGameplayEffectSpecHandle> TargetEffectSpecs;// 一组真正的GE句柄
};
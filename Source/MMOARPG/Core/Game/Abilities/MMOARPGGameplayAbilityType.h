#pragma once

#include "CoreMinimal.h"
#include "MMOARPGGameplayAbilityType.generated.h"

/**
 * MMO-GE组 
 * 明确指明由某个GA诱发的MMO-GE组(携带一组buff).
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
 * MMO-GE包 
 * 明确指明由某个GA诱发的 MMO-GE包
 * MMO-GE包内携带 UE-GE目标数据句柄 以及 一组真正的GE句柄
 */
USTRUCT(BlueprintType)
struct FMMOARPGGameplayEffectSpec
{
	GENERATED_BODY()

public:
	// GE作用的数据集
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		FGameplayAbilityTargetDataHandle TargetHandleData;// GE目标数据句柄
	// 一组真正的GE句柄.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		TArray<FGameplayEffectSpecHandle> TargetEffectSpecs;// 一组真正的GE句柄
};
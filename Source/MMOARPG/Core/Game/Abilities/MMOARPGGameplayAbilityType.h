#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "MMOARPGGameplayAbilityType.generated.h"

/**
 * buff的使用分型
 */
UENUM(BlueprintType)
enum class EGameplayEffectUseType : uint8
{
	// 碰撞后释放buff 给目标
	RELEASE_TARGET_COLLISION		UMETA(DisplayName = "Release to Taget after collision"),
	// 碰撞后释放buff 给自己
	RELEASE_OWN_COLLISION			UMETA(DisplayName = "Release to own after collision"),
};

/**
 * MMO项目的单体buff
 */
USTRUCT(BlueprintType)
struct FMMOARPGGameplayEffect
{
	GENERATED_BODY()
public:
	FMMOARPGGameplayEffect();

	// 被动buff类型 是释放到目标还是自己
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MMOARPGGameplayEffect)
		EGameplayEffectUseType GameplayEffectUseType;

	// 具体的单体GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MMOARPGGameplayEffect)
		TSubclassOf<UGameplayEffect> EffectClass;
};

/**
 * MMO项目的buff组
 * 明确指明由某个GA诱发的MMO-GE组(携带一组buff).
 */
USTRUCT(BlueprintType)
struct FMMOARPGGameplayEffects
{
	GENERATED_BODY()

public:
	// 一组 MMOBUFF
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		TArray<FMMOARPGGameplayEffect> MMOGEs;// 一组 MMOBUFF
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
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "../MMOARPGGameType.h"
#include "CharacterSkillTable.generated.h"

class UGameplayAbility;

/**
 * 与DataTable搭配使用(本质上是DT的一行).
 * 用以管理技能的结构体.
 */
USTRUCT(BlueprintType)
struct FCharacterSkillTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterSkillTable();
// public:
// 	TSubclassOf<UGameplayAbility>* FindComboAttack(const FName& InKey);// 用Key获取缓存池:combo形式
// 	TSubclassOf<UGameplayAbility>* FindSkillAttack(const FName& InKey);// 用Key获取缓存池:skill形式
// 	TSubclassOf<UGameplayAbility>* FindLimbs(const FName& InKey);// 用Key获取缓存池:肢体行为

public:
	// 可选用的人物号(比如主角,仆从1,仆从2)
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		int32 ID;// 可选用的人物号(比如主角,仆从1,仆从2)
	
	// 技能图标
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		UTexture2D* Icon;// 技能图标
	
	// 技能冷却CD
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		float CD;// 技能冷却CD

	// 此技能分类类型
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		EMMOARPGSkillType SkillType;

	// 技能属性提示
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		FText SkillAttributeTip;// 技能属性提示

	// 具体技能类
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		TSubclassOf<UGameplayAbility> GameplayAbility;// 具体技能类

// 	// 普攻(连招)形式的攻击 缓存池, 存在于负责技能的DTRow;
// 	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
// 		TArray<TSubclassOf<UGameplayAbility>> ComboAttack;// 攻击缓存池:连招形式, 存在于负责技能的DTRow;
// 	
// 	// 技能形式的攻击 缓存池, 存在于负责技能的DTRow;
// 	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
// 		TArray<TSubclassOf<UGameplayAbility>> SkillAttack;// 攻击缓存池:技能形式, 存在于负责技能的DTRow;
// 
// 	// 和肢体行为相关的一些能力; 例如受击, 死亡, 嘲讽 打招呼
// 	UPROPERTY(EditDefaultsOnly, Category = "CharacterLimbs")
// 		TArray<TSubclassOf<UGameplayAbility>> Limbs;// 肢体行为缓存池; 
};
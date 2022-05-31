#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterAnimTable.generated.h"

/**
 * 用以管理动画蒙太奇的表.
 */
USTRUCT(BlueprintType)
struct FCharacterAnimTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterAnimTable();

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAnim")
		int32 ID;
	
	// 蒙太奇: 切换战斗姿势;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAnim")
		UAnimMontage* SwitchFightMontage;
};
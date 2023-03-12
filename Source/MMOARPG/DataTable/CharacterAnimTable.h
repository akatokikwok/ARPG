#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterAnimTable.generated.h"

/**
 * 与DataTable搭配使用(本质上是DT的一行).
 * 用以管理动画蒙太奇的结构体.
 */
USTRUCT(BlueprintType)
struct FCharacterAnimTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterAnimTable();

	// 蒙太奇序号;原理是 1个ID关联1个蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAnim")
		int32 ID;
	
	// 蒙太奇: 切换战斗姿势;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAnim")
		UAnimMontage* SwitchFightMontage;

	// 蒙太奇: 攀爬姿势;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAnim")
		UAnimMontage* ClimbingMontage;

	// 挑飞后起身动画
	UPROPERTY(EditDefaultsOnly, Category = "CharacterAnim")
		UAnimMontage* GetUpMontage;
};
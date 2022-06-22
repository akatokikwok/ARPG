#pragma once
#include "CoreMinimal.h"
#include "SimpleComboType.generated.h"

/**
 * 连击检测感知器. 
 * 负责检测combo连击.
 */
USTRUCT( BlueprintType)
struct SIMPLECOMBAT_API FSimpleComboCheck
{
	GENERATED_USTRUCT_BODY()
public:
	FSimpleComboCheck();

	// 在总段数内循环递增刷新段号.
	void UpdateComboIndex();
	// Press()激发连击触发器黑盒.
	void Press();
	// 主动结束连击触发器黑盒.
	void Released();
	// 复位总的状态.
	void Reset();

/// //////////////////////////////////////////////////////////////////////////
public:
	// 第几段combo
	UPROPERTY(EditDefaultsOnly, Category = "ComboAttack")
		int32 ComboIndex;
	// 是否长按
	UPROPERTY(EditDefaultsOnly, Category = "ComboAttack")
		bool bLongPress;
	// 是否短按
	UPROPERTY(EditDefaultsOnly, Category = "ComboAttack")
		bool bShortPress;
	
	// 给人的类继承的战斗IInterface.
	class ISimpleComboInterface* Character_CombatInterface;

	// Combo支持的最大段数.
	UPROPERTY()
		int32 MaxIndex;
	
	// GA名字; 由外界主动注册.
	UPROPERTY()
		FName ComboKey_GA;
};
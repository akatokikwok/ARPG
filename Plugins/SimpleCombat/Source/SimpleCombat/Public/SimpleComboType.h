#pragma once
#include "CoreMinimal.h"
#include "SimpleComboType.generated.h"

/**
 * 负责检测combo连击的检测类.
 */
USTRUCT( BlueprintType)
struct SIMPLECOMBAT_API FSimpleComboCheck
{
	GENERATED_USTRUCT_BODY()
public:
	FSimpleComboCheck();

	//
	void UpdateComboIndex();
	//
	void Press();
	//
	void Released();
	//
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
	class ISimpleComboInterface* Character;

	// Combo支持的最大段数.
	UPROPERTY()
		int32 MaxIndex;
	
	// 一个FName管理多个连招.
	UPROPERTY()
		FName ComboKey;
};
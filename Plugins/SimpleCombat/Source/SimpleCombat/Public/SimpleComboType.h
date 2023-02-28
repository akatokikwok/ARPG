#pragma once
#include "CoreMinimal.h"
#include "SimpleComboType.generated.h"
class UAnimMontage;


/**
 * 碰撞伤害类型
 */
UENUM(BlueprintType)
enum class EHitCollisionType :uint8
{
	HITCOLLISIONTYPE_SHORT_RANGE_ATTACK UMETA(DisplayName = "Short range attack"),		//近程攻击
	HITCOLLISIONTYPE_DIRECT_LINE 		UMETA(DisplayName = "Direct Line"),				//无障碍直线攻击
	HITCOLLISIONTYPE_LINE				UMETA(DisplayName = "Line"),					//非跟踪类型，类似手枪子弹；
	HITCOLLISIONTYPE_TRACK_LINE			UMETA(DisplayName = "Track Line"),				//跟踪类型
	HITCOLLISIONTYPE_RANGE_LINE			UMETA(DisplayName = "Range Line"),				//范围伤害，丢手雷；
	HITCOLLISIONTYPE_RANGE				UMETA(DisplayName = "Range"),					//范围伤害，类似自爆；
	HITCOLLISIONTYPE_CHAIN				UMETA(DisplayName = "Chain"),					//链条类型，持续伤害类型;
};


/**
 * 持续施法消耗黑盒
 */
USTRUCT(BlueprintType)
struct SIMPLECOMBAT_API FContinuousReleaseSpell
{
	GENERATED_USTRUCT_BODY()
public:
	FContinuousReleaseSpell();
	void Reset();
	void Press();
	void Released();

public:
	UPROPERTY(EditDefaultsOnly, Category = ContinuousReleaseSpell)
		int32 ContinuousReleaseSpellIndex;

	UPROPERTY(EditDefaultsOnly, Category = ContinuousReleaseSpell)
		UAnimMontage* AnimMontage;

	class ISimpleComboInterface* Character;

	UPROPERTY()
		UObject* BuffPtr;
};


/**
 * 连击检测感知器.
 * 负责检测combo连击.
 */
USTRUCT(BlueprintType)
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

// 判断2个黑盒检测器是否相等
inline bool operator==(const FSimpleComboCheck& L, const FSimpleComboCheck& R)
{
	return L.ComboKey_GA == R.ComboKey_GA;
}
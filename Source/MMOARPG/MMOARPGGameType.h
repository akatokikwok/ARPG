#pragma once

#include "CoreMinimal.h"
#include "MMOARPGGameType.generated.h"

/* 人物行为姿态.*/
UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	NORMAL_STATE	UMETA(DisplayName = "Normal"),// 普通
	FIGHT_STATE		UMETA(DisplayName = "Fight"),// 战斗
	FLIGHT_STATE	UMETA(DisplayName = "Flight"),// 飞行
	CLIMB_STATE		UMETA(DisplayName = "Climb"),// 攀爬
	SWIMMING_STATE	UMETA(DisplayName = "Swimming"),// 游泳
};

/**
 * 翻滚种类枚举.
 */
UENUM(BlueprintType)
enum class EDodgeFly : uint8
{
	DODGE_NONE		UMETA(DisplayName = "None"),
	DODGE_LEFT		UMETA(DisplayName = "Left"),// 空中左翻滚
	DODGE_RIGHT		UMETA(DisplayName = "Right"),// 空中右翻滚
};

/**
 * 攀爬或者翻越状态.
 */
UENUM(BlueprintType)
enum class EClimbingState : uint8
{
	CLIMBING_NONE					UMETA(DisplayName = "None"),
	CLIMBING_CLIMBOVEROBSTACLES		UMETA(DisplayName = "Climb Over Obstacles"),// 翻越矮小障碍物.
	CLIMBING_WALLCLIMBING			UMETA(DisplayName = "Wall Climbing"),// 翻墙
	CLIMBING_CLIMBING				UMETA(DisplayName = "Climbing"),// 攀岩.
	CLIMBING_TOGROUND				UMETA(DisplayName = "Climbing to Ground"),// 攀岩.
	CLIMBING_TOTOP					UMETA(DisplayName = "To Top"),
	CLIMBING_DROP					UMETA(DisplayName = "Drop"),
	CLIMBING_TURN					UMETA(DisplayName = "Turn"),
};

/**
 * 攀爬跳这种姿态的各型枚举.
 */
UENUM(BlueprintType)
enum class EClimbingMontageState : uint8
{
	CLIMBING_DASH_DR_RM							UMETA(DisplayName = "Climbing Dash Down Right"),
	CLIMBING_DASH_D_RM							UMETA(DisplayName = "Climbing Dash Down"),
	CLIMBING_DASH_DL_RM							UMETA(DisplayName = "Climbing Dash Down Left"),
	CLIMBING_DASH_L_RM							UMETA(DisplayName = "Climbing Dash Left"),
	CLIMBING_DASH_R_RM							UMETA(DisplayName = "Climbing Dash Right"),
	CLIMBING_DASH_U_RM							UMETA(DisplayName = "Climbing Dash UP"),
	CLIMBING_DASH_UL_RM							UMETA(DisplayName = "Climbing Dash UP Left"),
	CLIMBING_DASH_UR_RM							UMETA(DisplayName = "Climbing Dash UP Right"),
	CLIMBING_CLIMB_UP_AT_TOP					UMETA(DisplayName = "Climb Up At Top"),
	CLIMBING_ALS_N_MANTLE_1M_LH					UMETA(DisplayName = "Climb Left Turn"),
	CLIMBING_ALS_N_MANTLE_1M_RH					UMETA(DisplayName = "Climb Right Turn"),
	CLIMBING_ALS_N_MANTLE_2M					UMETA(DisplayName = "Climb Turn"),
	CLIMBING_DROP_RM							UMETA(DisplayName = "Climb Drop"),
	CLIMBING_TURN_CORNER_OUTSIDE_R_RM			UMETA(DisplayName = "Trun Corner Outside Right"),
	CLIMBING_TURN_CORNER_OUTSIDE_L_RM			UMETA(DisplayName = "Turn Corner Outside Left"),
	CLIMBING_TURN_CORNER_INSIDE_R_RM			UMETA(DisplayName = "Turn Corner Inside Right"),
	CLIMBING_TURN_CORNER_INSIDE_L_RM			UMETA(DisplayName = "Turn Corner Inside Left"),
	CLIMBING_DASH_MAX							UMETA(DisplayName = "Max"),
};


/**
 * 刷新切换BOOL状态的结构, 用于飞行姿态中的疾飞和落地.
 */
struct FResetBool
{
	FResetBool()
		: bSet(false)
		, Time(0.f)
	{}

	void Tick(float DeltaTime)
	{
		if (Time > 0.f) {
			Time -= DeltaTime;
			if (Time <= 0.f) {/* 计时结束了就播委托并刷新状态. */
				Fun.ExecuteIfBound();
				Time = 0.f;// 计时结束, 本计时设为0.0秒.
				bSet = false;// 计时结束, 本动作设为假
			}
		}
	}
	// 重载 = ; 刷新此动作布尔状态.
	FResetBool& operator=(bool bNewSet)
	{
		bSet = bNewSet;
		return *this;
	}
	// 重载 = ; 刷新此动作延时总时长.
	FResetBool& operator=(float InNewTime)
	{
		Time = InNewTime;
		return *this;
	}
	// 重载 *, 拿此动作bool状态.
	bool operator*()
	{
		return bSet;
	}
	// 显示重载bool 符号, 达到if()验证的效果;  拿此动作的布尔值.
	explicit operator bool()
	{
		return bSet;
	}
	// 计时结束代理.
	FSimpleDelegate Fun;

protected:
	bool bSet;
	float Time;// 延时的总时长.
};
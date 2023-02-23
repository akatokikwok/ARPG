#pragma once

#include "CoreMinimal.h"
#include "MMOARPGGameType.generated.h"

/**
 * 技能分类类型
 */
UENUM(BlueprintType)
enum class EMMOARPGSkillType : uint8
{
	// None, 0
	NONE_SKILLS								UMETA(DisplayName = "None"),
	// 1 2 3 4 5通用技能,这里枚举强制赋值为5,即非从[1]开始,而是跳到了5
	GENERAL_SKILLS = 5						UMETA(DisplayName = "General Skills"),
	// 6 从天而降条件技能
	DROP_FROM_THE_CLOUDS_SKILL				UMETA(DisplayName = "Drop From The Clouds"),
	// 7 闪避技能 鼠标右键的那个闪避
	DODGE_SKILL								UMETA(DisplayName = "Dodge Skill"),
	// 8 地面连击技能
	COMBO_GROUND_SKILL						UMETA(DisplayName = "Combo Ground Skill"),
	// 9 空中连击技能
	COMBO_AIR_SKILL							UMETA(DisplayName = "Combo Air Skill"),
	// 10 条件技能
	CONDITIONAL_SKILLS						UMETA(DisplayName = "Conditional skills"),
	// 11 冲刺技能
	SPRINT_SKILLS							UMETA(DisplayName = "Sprint Skill"),
	// 其他件技能,诸如死亡/受击那种状态技能
	OTHER_SKILLS							UMETA(DisplayName = "Other skills"),
};

/**
 * 弹窗消息类型枚举
 */
enum class EPopupMsgType :uint8
{
	POPUP_MSG_RESURRECTION,// 重生
};

/**
 * 游戏各阶段状态枚举.
 */
enum EMMOARPGGameStatusType
{
	MMOARPG_PINCH_REQUEST_STATUS = 1,  // 捏脸注册
	MMOARPG_DATA_ACQUISITION_PHASE = 2,// 数据请求注册
	MMOARPG_IN_GAME = 4,			   // 游戏阶段
};

/**
 * 敌我阵营划分枚举.
 */
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	CHARACTER_NONE				 UMETA(DisplayName = "None"),			//1 主角
	CHARACTER_PLAYER_MAIN		 UMETA(DisplayName = "Lead"),			//1 主角
	CHARACTER_PARTNER			 UMETA(DisplayName = "Partner"),		//2 ~ 4096 //配角
	CHARACTER_NPC_RESIDENT		 UMETA(DisplayName = "Resident"),		//4097 ~ 8119 //居民
	CHARACTER_NPC_GUARDIAN		 UMETA(DisplayName = "Guardian"),		//8120 ~ 16423 //守卫者
	CHARACTER_MONSTER			 UMETA(DisplayName = "Monster"),		//16424~30000	//小怪
	CHARACTER_BOSS				 UMETA(DisplayName = "Boss"),			//30001			//Boss
};

// 攻击形式来源枚举.
UENUM(BlueprintType)
enum class EMMOARPGGameplayAbilityType : uint8
{
	GAMEPLAYABILITY_NONE			UMETA(DisplayName = "None"),// 
	GAMEPLAYABILITY_SKILLATTACK		UMETA(DisplayName = "SkillAttack"),// 从属技能形式的攻击.
	GAMEPLAYABILITY_COMBOATTACK		UMETA(DisplayName = "ComboAttack"),// 从属combo形式的攻击.
	GAMEPLAYABILITY_LIMBS			UMETA(DisplayName = "Limbs"),// 肢体动作的技能或能力状态;  例如死亡, 挨打
};

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
 * 攀爬系统的哪个姿态.
 */
UENUM(BlueprintType)
enum class EClimbingState : uint8
{
	CLIMBING_NONE					UMETA(DisplayName = "None"),
	CLIMBING_CLIMBOVEROBSTACLES		UMETA(DisplayName = "Climb Over Obstacles"),// 翻越矮小障碍物.
	CLIMBING_WALLCLIMBING			UMETA(DisplayName = "Wall Climbing"),// 翻墙
	CLIMBING_CLIMBING				UMETA(DisplayName = "Climbing"),// 攀岩.
	CLIMBING_TOGROUND				UMETA(DisplayName = "Climbing to Ground"),// 落地
	CLIMBING_TOTOP					UMETA(DisplayName = "To Top"),// 爬顶
	CLIMBING_DROP					UMETA(DisplayName = "Drop"),// 主动坠落
	CLIMBING_TURN					UMETA(DisplayName = "Turn"),// 转向切换
};

/**
 * 攀岩-跳 行为的哪个子型
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
	CLIMBING_TURN_CORNER_FROMAIR_RM				UMETA(DisplayName = "Jump To Climbing"),// 跳在空中然后攀岩
	CLIMBING_TURN_CORNER_FROMGROUND_RM			UMETA(DisplayName = "Walk To Climbing"),// 从走路接触到墙然后攀岩
	CLIMBING_DASH_MAX							UMETA(DisplayName = "Max"),
};

/**
 * 攀岩-拐弯 行为的哪个子型
 */
UENUM(BlueprintType)
enum class EClimbingTurnState : uint8
{
	TURN_NONE,
	OUTSIDE_RIGHT,
	OUTSIDE_LEFT,
	INSIDE_RIGHT,
	INSIDE_LEFT,
};

/**
 * 刷新切换BOOL状态的结构, 多用途,可绑定lambda
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
#pragma once

#include "CoreMinimal.h"
#include "MMOARPGGameType.generated.h"

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	NORMAL_STATE	UMETA(DisplayName = "Normal"),// 普通
	FIGHT_STATE		UMETA(DisplayName = "Fight"),// 战斗
	FLIGHT_STATE	UMETA(DisplayName = "Flight"),// 飞行
	CLIMB_STATE		UMETA(DisplayName = "Climb"),// 攀爬
	SWIMMING_STATE	UMETA(DisplayName = "Swimming"),// 游泳
};

UENUM(BlueprintType)
enum class EDodgeFly : uint8
{
	DODGE_NONE		UMETA(DisplayName = "None"),
	DODGE_LEFT		UMETA(DisplayName = "Left"),
	DODGE_RIGHT		UMETA(DisplayName = "Right"),
};
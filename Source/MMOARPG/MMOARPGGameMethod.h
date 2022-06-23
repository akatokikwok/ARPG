/**
 * 本页存放一些用于游戏框架内的通用方法或者通用算法.
 */

#pragma once
#include "CoreMinimal.h"
class AMMOARPGCharacterBase;

namespace MMOARPGGameMethod
{
	/**
	 * 查找半径内最近的敌对目标.
	 */
	AMMOARPGCharacterBase* FindTarget(AMMOARPGCharacterBase* InThis, float InRange = 520.f);
}
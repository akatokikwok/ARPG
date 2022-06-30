/**
 * 本页存放一些用于游戏框架内的通用方法或者通用算法.
 */

#pragma once
#include "CoreMinimal.h"
#include "MMOARPGGameType.h"
//  #include "MMOARPGTagList.h"// 测试此由SimpleCombatEditor module而反射生成的头文件是否通过编译.
class AMMOARPGCharacterBase;

namespace MMOARPGGameMethod
{
	/**
	 * 查找半径内最近的敌对目标.
	 */
	AMMOARPGCharacterBase* FindTarget(AMMOARPGCharacterBase* InThis, float InRange = 520.f);

	/** 敌我识别用, 求取人物身份类型 */
	ECharacterType GetCharacterType(int32 InCharacterID);
}
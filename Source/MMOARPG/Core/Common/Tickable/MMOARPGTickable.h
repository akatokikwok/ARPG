// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

/**
 * 主要为了游戏中需要被单独tick的 单例tick 如果在项目里面 多个客户端可能会同一帧tick该单例
 */
class FMMOARPGTickable :public FTickableGameObject
{
public:
	static FMMOARPGTickable* Get();
	static void Destroy();
public:
	// 本类的tick使用协程形式的tick
	virtual void Tick(float DeltaTime);

	virtual TStatId GetStatId() const;

private:
	static FMMOARPGTickable* MMOARPGTickableSinglePtr;
};
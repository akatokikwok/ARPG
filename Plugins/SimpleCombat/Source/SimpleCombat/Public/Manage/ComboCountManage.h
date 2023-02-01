#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
class UUI_ComboCount;

/**
 * combo计数单例 manager
 */
class SIMPLECOMBAT_API FComboCountManage : public FTickableGameObject
{
public:
	FComboCountManage();
	static FComboCountManage* Get();
	static void Destroy();

	// Monitored by main thread 
	virtual void Tick(float DeltaTime);
	//
	virtual TStatId GetStatId() const;

public:
	// 创建连打计数UI
	UUI_ComboCount* GetComboCount(UWorld* InWorld, TSubclassOf<UUI_ComboCount> InClass);
	// 构建出连打计数UI并同步执行UI表现
	void Play(UWorld* InWorld, TSubclassOf<UUI_ComboCount> InClass);

protected:
	static FComboCountManage* Instance;// 单例指针

	int32 ComboNumber;// 连打计数

	float TimeDelta;// 存续时长
	float MaxDeltaTime;// 最大存续时长
};
#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Core/Element/SimpleFootIK.h"
#include "SimpleAdvancedAnimationType.h"

/**
 * 管理高级动画、IK的架构层
 * 单例模式
 * 拥有一些控制方法，管理帧动画，帧IK.
 */
class FSimpleAdvancedAnimSystem : public FTickableGameObject
{
public:
	static FSimpleAdvancedAnimSystem* Get();
	static void Destroy();
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	/** 手动构建 IK和与之匹配的Hanle. */
	FSAAHandle CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float TraceDistance = 50.f, float InterpSpeed = 18.f, float InTraceStart = 50.f);

	// 找到指定Hanle的 IK.
	FSimpleFootIK* FindFootIK(const FSAAHandle InKey);
	// 找到指定Handle 的 IK 的 指定 脚名 的 IK数据.
	FFootIKInfo* FindFootIKInfo(const FSAAHandle InKey, const FName& InKeyName);
	// 找到指定Handle 的 IK 的 指定 脚名 的 IK数据里的偏移值.
	float FindOffset(const FSAAHandle InKey, const FName& InKeyName);

public:
	TMap<FSAAHandle, FSimpleFootIK> FootIKs;// FootIK查询表.

protected:
	static FSimpleAdvancedAnimSystem* AdvancedAnimSystem;// 单例指针.


};
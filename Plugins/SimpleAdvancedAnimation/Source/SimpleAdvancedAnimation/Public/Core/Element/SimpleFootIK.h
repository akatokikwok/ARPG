#pragma once

#include "CoreMinimal.h"
#include "SimpleAdvancedAnimationType.h"

/**
 * 完整的FootIK结构.
 */
struct FSimpleFootIK
{
public:
	FSimpleFootIK();
	/** 每帧 解算脚部的IK. */
	void Tick(float DeltaTime);

	// 允许外部使用的注册.
	void Init(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceDistance = 50.f, float InInterpSpeed = 18.f, float InTraceStart = 50.f);

	// 找特定脚的Ik信息里的偏移值.
	float FindOffset(const FName& InKeyName);
	// 找特定脚的IK信息.
	FFootIKInfo* FindFootIKInfo(const FName& InKeyName);

	// 拿取 人销毁状态.
	bool IsPendingKill() { return bPendingKill; }

protected:
	// 执行射线检测IK, 返回某只脚的偏移; 若检测失败返回0.
	// InTraceDistance是人为设定的. 要检测多深.
	float FootTrace(const FName& BoneName, float InTraceDistance);

protected:
	ACharacter* Character;// 人.
	TMap<FName, FFootIKInfo> IKInfos;// <名称-单只脚IK信息>查询表.
	float TraceDistance;// 人设的射线检测深度.
	float InterpSpeed;// 渐变插值快慢速度.
// 	float TraceStart;//
	bool bPendingKill;// 人销毁通知.
};
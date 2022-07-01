// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MMOARPGAIControllerBase.generated.h"

class AMMOARPGCharacterBase;
/**
 * MMO项目的 AIC基类
 */
UCLASS()
class MMOARPG_API AMMOARPGAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

public:
	// 设定黑板组件里的敌人.
	virtual void SetTargetForce(AMMOARPGCharacterBase* InTarget);
	// 获取黑板组件里的敌人.
	AMMOARPGCharacterBase* GetTarget();
	// AIC控制自己去 搜寻敌人.
	virtual AMMOARPGCharacterBase* FindTarget();
	// 让AIC去控制自身执行攻击
	virtual void Attack(AMMOARPGCharacterBase* InTarget);
	// 让AIC去控制自身执行攻击(按标签)
	virtual void Attack(const FName& InTag);
	// 设定黑板里小怪出生位置
	void SetOriginalLocation(const FVector& InNewLocation);
};

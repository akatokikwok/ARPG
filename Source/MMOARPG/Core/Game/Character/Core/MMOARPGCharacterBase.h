// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../DataTable/CharacterAnimTable.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "MMOARPGCharacterBase.generated.h"

UCLASS()
class MMOARPG_API AMMOARPGCharacterBase : public ACharacter, public ISimpleCombatInterface
{
	GENERATED_BODY()
public:
	friend class AMMOARPGGameMode;// 人物基类的一切数据均提供GM访问.
public:
	// Sets default values for this character's properties
	AMMOARPGCharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// C++版.重载自 ISimpleCombatInterface::AnimSignal.
	virtual void AnimSignal(int32 InSignal) override;
	// 蓝图里实现的 AnimSignal函数. 名字特殊定制一下.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName = "AnimSignal_BPVersion", Category = "Anim Event")
		void K2_AnimSignal(int32 InSignal);
	//
	FORCEINLINE bool IsFight() { return bFight; }
	// 拿取蒙太奇DT里的 行数据.
	FORCEINLINE FCharacterAnimTable* GetAnimTable() { return AnimTable; }
	//
	FORCEINLINE int32 GetID() { return ID; }
	//
	FORCEINLINE int32 GetUserID() { return UserID; }

protected:
	// 同步变量需要重写的方法.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* 客户端通知DS上执行修改; 
	 * 用以修改bFight字段. 
	 * UFUNCTION(Server 运行在服务器 , Reliable 同步的,可靠的,TCP
	 */
	UFUNCTION(Server, Reliable)
		void SwitchFightOnServer(bool bNewFight);

	// bFight更新时 响应的RPC.
	UFUNCTION()
		virtual void OnRep_FightChanged();

protected:
	// 是否启用战斗姿势.
	UPROPERTY(ReplicatedUsing = OnRep_FightChanged)
		bool bFight;
	// 蒙太奇动画数据点 ID, 用户去配置的ID.
	UPROPERTY(EditDefaultsOnly, Category = "Character")
		int32 ID;
	// 用户ID.
	UPROPERTY()
		int32 UserID;

	// 关联动画蒙太奇DT的某 行数据.
	FCharacterAnimTable* AnimTable;
};

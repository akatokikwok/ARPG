// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../DataTable/CharacterAnimTable.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "../../../../MMOARPGGameType.h"
#include "../../../Component/FlyComponent.h"
#include "MMOARPGCharacterBase.generated.h"

UCLASS()
class MMOARPG_API AMMOARPGCharacterBase : public ACharacter, public ISimpleCombatInterface
{
	GENERATED_BODY()
private:
	friend class AMMOARPGGameMode;// 人物基类的一切数据均提供GM访问.

	// 飞行组件.
	UPROPERTY()
		UFlyComponent* FlyComponent;
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
	// 拿取人物姿态.
	FORCEINLINE ECharacterActionState GetActionState() { return ActionState; }
	// 拿取蒙太奇DT里的 行数据.
	FORCEINLINE FCharacterAnimTable* GetAnimTable() { return AnimTable; }
	// 拿取 游玩人物专属ID.
	FORCEINLINE int32 GetID() { return ID; }
	// 拿取 用户ID.用户去配置的ID.
	FORCEINLINE int32 GetUserID() { return UserID; }
	// 拿取飞行组件.
	FORCEINLINE UFlyComponent* GetFlyComponent() { return FlyComponent; }
	// 拿附属的相机,虚接口.
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return nullptr; }

	// 强制刷新到指定姿态. 若和新姿态相同则还原为normal.
	void ResetActionState(ECharacterActionState InNewActionState);
protected:
	// 同步变量需要重写的方法.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* RPC到 DS上执行修改;
	 * 用以修改ActionState字段.
	 * UFUNCTION(Server 运行在服务器 , Reliable 同步的,可靠的,TCP
	 */
	UFUNCTION(Server, Reliable)
		void SwitchActionStateOnServer(ECharacterActionState InActionState);

	// ActionState更新时 响应的RPC.
	UFUNCTION()
		virtual void OnRep_ActionStateChanged();

protected:
	// 人物动作状态.
	UPROPERTY(ReplicatedUsing = OnRep_ActionStateChanged)
		ECharacterActionState ActionState;

	// 最后一次切换到的人物动作状态.
	UPROPERTY()
		ECharacterActionState LastActionState;

	// 游玩人物专属ID.
	UPROPERTY(EditDefaultsOnly, Category = "Character")
		int32 ID;
	// 用户ID.用户去配置的ID.
	UPROPERTY()
		int32 UserID;

	// 关联动画蒙太奇DT的某 行数据.
	FCharacterAnimTable* AnimTable;

};

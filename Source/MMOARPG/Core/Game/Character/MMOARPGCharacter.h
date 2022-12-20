// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/MMOARPGCharacterBase.h"
#include "MMOARPGCharacter.generated.h"

UCLASS(config = Game)
class AMMOARPGCharacter : public AMMOARPGCharacterBase/*, public IKneadingInterface*/
{
	GENERATED_BODY()
public:
	AMMOARPGCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//////////////////////////////////////////////////////////////////////////

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** 覆写虚接口, 拿附属的相机. **/
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	/** 抽刀收刀的切换逻辑; 变量bFight在DS被修改之后发生的逻辑; 可供OnRep_FightChanged调用. */
	void FightChanged();

	/** 攀爬跳姿势的切换逻辑. */
	virtual void ClimbingMontageChanged(EClimbingMontageState InJumpState) override;

protected:
	// 打印指定时长的指定语句.
	void Print(float InTime, const FString& InString);
	/** Resets HMD orientation in VR. */
	void OnResetVR();
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector InLocation);
	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector InLocation);

	/** 切换战斗姿势启用/禁用. */
	void SwitchFight();

	/** 当字段被DS刷新后,本机/其他的客户端Player做出的反应. */
	virtual void OnRep_ActionStateChanged() override;

	/* 各自运动姿态系统里的切换(如站立切到飞行, 游泳切到潜泳). RPC 客户端到DS. */
	UFUNCTION(Server, Reliable)
		void ActionSwitching();
	/* 在服务器上做一些逻辑,做完后再广播, 通知到客户端. 使用NetMulticast宏. */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastActionSwitching();
	/* RPC在服务器 命令其 广播急速运动动作至其他服务器.*/
	UFUNCTION(Server, Reliable)
		void Fast();
	/* 服务器上广播 那些急速动作. 在服务器上做一些逻辑,做完后再广播, 通知到客户端. 使用NetMulticast宏. */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastFast();
	/* 松开急速执行某运动动作. */
	UFUNCTION(Server, Reliable)
		void FastReleased();
	/* 广播 停止加速*/
	UFUNCTION(NetMulticast, Reliable)
		void MulticastFastReleased();

	/* 空中左翻滚.*/
	UFUNCTION(Server, Reliable)
		void DodgeLeft();
	/* 服务器上广播左躲闪. */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastDodgeLeft();
	/* 空中右翻滚.*/
	UFUNCTION(Server, Reliable)
		void DodgeRight();
	/* 服务器上广播右躲闪. */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastDodgeRight();

	/** RPC人物某运动动作减速 */
	UFUNCTION(Server, Reliable)
		void SlowDown();
	/** 人物某运动动作减速服务器广播 */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastSlowDown();
	/** RPC人物某运动动作减速 松开 */
	UFUNCTION(Server, Reliable)
		void SlowDownReleased();
	/** 人物某运动动作减速服务器广播 松开 */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastSlowDownReleased();

	/** 用于攀爬系统的 跳爬 */
	void CharacterJump();

	/** 用于攀爬系统的 跳爬释放 */
	void CharacterStopJumping();

	/** RPC服务器 "执行攀岩跳的效果". */
// 	UFUNCTION(Server, Reliable)
// 		void CharacterJumpToServer();

	/** 广播 "攀岩跳" */
// 	UFUNCTION( NetMulticast, Reliable)
// 		void MulticastCharacterJump();

/// 技能相关
public:
	// RPC在服务器, 由客户端向CS发送属性集请求.
	UFUNCTION(Server, Reliable)
		void GetCharacterDataRequests();

	// RPC在服务器, 左mouse按下后续
	UFUNCTION(Server, Reliable)
		void MouseLeftClick();
	// RPC在服务器, 右mouse按下后续
	UFUNCTION(Server, Reliable)
		void MouseRightClick();
	// RPC在服务器, 左mouse松开后续
	UFUNCTION(Server, Reliable)
		void MouseLeftClickReleased();
	// RPC在服务器, 右mouse松开后续
// 	UFUNCTION(Server, Reliable)
	void MouseRightClickReleased();

	// 按键冲刺, RPC在服务器.
	UFUNCTION(Server, Reliable)
		void Sprint();// 按键冲刺, RPC在服务器.

	/** 覆盖CombatInterface接口, 如若信号值设定2,则重置触发器黑盒. */
	virtual void AnimSignal(int32 InSignal) override;

	// 覆写GetTarget接口. 读取敌对目标人物或者怪物
	virtual AActor* GetTarget() override;

public:
	// 为本人物升级.
	virtual void UpdateLevel(float InLevel) override;

public:
	// 处理人的血量; 覆写
	virtual void HandleHealth(AMMOARPGCharacterBase* InstigatorPawn, AActor* DamageCauser, const struct FGameplayTagContainer& InTags, float InNewValue) override;
	
	// 处理人的蓝量; 覆写
	virtual void HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue) override;

	// 覆写处理经验值接口
	virtual void HandleExp(const struct FGameplayTagContainer& InTags, float InNewValue) override;

public:
	// 创建重生弹窗
	UFUNCTION(Client, Reliable)
	void CreateResurrectionWindowsClient();

	// 服务端执行技能形式的技能攻击(需指定一个槽号)
	UFUNCTION(Server, Reliable)
	void SKillAttackOnServer(int32 InSlot);

public:/// 关于技能槽的一些接口

	/** 从横框到技能页: 移动 */
	UFUNCTION(Server, Reliable)
		void SKillSlotMoveToSkillTable(int32 InSlot);

	/** 从横框到技能页: 交换 */
	UFUNCTION(Server, Reliable)
		void SKillSlotSwapSkillTable(int32 InRemoveSlot, const FName& InTag);

	/** 从技能页到横框: 移动 */
	UFUNCTION(Server, Reliable)
		void SKillTableSlotMoveToSkillSlot(const FName& InTag, int32 InSlot);

	/** 从技能页到横框: 交换 */
	UFUNCTION(Server, Reliable)
		void SKillTableSlotSwapSkillSlot(int32 InRemoveSlot, const FName& InTag);

	/** 任意2个技能Slot之间的移动 */
	UFUNCTION(Server, Reliable)
		void SKillSlotMoveToNewSlot(int32 InASlot, int32 InBSlot);

	/** 横框内, 任意2个技能内2个技能槽交换 */
	UFUNCTION(Server, Reliable)
		void SillSlotSwap(int32 InASlot, int32 InBSlot);


	/// //////////////////////////////////////////////////////////////////////////
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
};


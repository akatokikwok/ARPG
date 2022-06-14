// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../DataTable/CharacterAnimTable.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "../../../../MMOARPGGameType.h"
#include "../../../Component/FlyComponent.h"
#include "../../../Component/SwimmingComponent.h"
#include "../../../Component/ClimbingComponent.h"
#include "../../../Component/FightComponent.h"
#include "AbilitySystemInterface.h"
#include "../../Abilities/MMOARPGAbilitySystemComponent.h"
#include "MMOARPGCharacterBase.generated.h"


/**
 * 持有IAbilitySystemInterface, 格斗接口, 等接口的人物基类.
 */
UCLASS()
class MMOARPG_API AMMOARPGCharacterBase : 
	public ACharacter, public ISimpleCombatInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()
private:
	friend class AMMOARPGGameMode;// 人物基类的一切数据均提供GM访问.
	friend class AMMOARPGCharacter;

	/**
	 * 飞行系统组件.(是一个强指针,引用它的那些要设计成弱指针.)
	 * 某份数据全局只能有一个,则需要设计为强指针.所有引用这份数据的设计为弱指针.
	 * 当强指针被释放了,引用它的那些弱指针们都会感应到,从而阻止出错.
	 */
	UPROPERTY(Category = "AMMOARPGCharacterBase", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UFlyComponent> FlyComponent;
	
	/** 游泳系统组件. */
	UPROPERTY(Category = "AMMOARPGCharacterBase", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<USwimmingComponent> SwimmingComponent;

	/** 攀爬系统组件. */
	UPROPERTY(Category = "AMMOARPGCharacterBase", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UClimbingComponent> ClimbingComponent;
	
	/** 战斗系统组件. 强指针,释放了的话会让其内部的弱指针成员感应到 */
	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UFightComponent> FightComponent;

	/** MMOARPG ASC组件. */
 	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
 		TObjectPtr<UMMOARPGAbilitySystemComponent> AbilitySystemComponent;
public:
	// Sets default values for this character's properties
	AMMOARPGCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
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
	// 拿取游泳组件.
	FORCEINLINE USwimmingComponent* GetSwimmingComponent() { return SwimmingComponent; }
	// 拿取攀爬组件.
	FORCEINLINE UClimbingComponent* GetClimbingComponent() { return ClimbingComponent; }
	// 拿取战斗系统组件.
	FORCEINLINE UFightComponent* GetFightComponent() { return FightComponent; }
	// 拿附属的相机,虚接口.
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return nullptr; }

	// 强制刷新到指定姿态. 若和新姿态相同则还原为normal.
	void ResetActionState(ECharacterActionState InNewActionState);

	/** 攀爬跳姿势的切换具体蒙太奇动画. */
	virtual void ClimbingMontageChanged(EClimbingMontageState InJumpState) {};
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

	// 重写基类; 落地(可能是飞行落地,或者是攀岩坠落落地)
	virtual void Landed(const FHitResult& Hit) override;

public:/// 技能相关

// 	// 添加技能
// 	FGameplayAbilitySpecHandle AddAbility(TSubclassOf<UGameplayAbility> InNewAbility);


/// //////////////////////////////////////////////////////////////////////////
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

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
#include "SimpleComboType.h"
#include "../../Abilities/MMOARPGAttributeSet.h"
#include "MMOARPGType.h"
#include "../../../../MMOARPGGameMethod.h"
#include "MMOARPGCharacterBase.generated.h"


/**
 * 持有IAbilitySystemInterface, 格斗接口, 等接口的人物基类.
 */
UCLASS()
class MMOARPG_API AMMOARPGCharacterBase : 
	public ACharacter, public ISimpleComboInterface, public IAbilitySystemInterface
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

	/** GAS属性集指针. */
	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UMMOARPGAttributeSet> AttributeSet;

public:
	// Sets default values for this character's properties
	AMMOARPGCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// 给特定的信号值,然后实现对应的notify逻辑; 覆写ISimpleCombatInterface::AnimSignal.
	virtual void AnimSignal(int32 InSignal) override;
	// 蓝图里实现的 AnimSignal函数. 名字特殊定制一下.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName = "AnimSignal_BPVersion", Category = "Anim Event")
		void K2_AnimSignal(int32 InSignal);

	// 强制刷新到指定姿态. 若和新姿态相同则还原为normal.
	void ResetActionState(ECharacterActionState InNewActionState);

	/** 攀爬跳姿势的切换具体蒙太奇动画. */
	virtual void ClimbingMontageChanged(EClimbingMontageState InJumpState) {};

	// 检查人物是否死亡.
	bool IsDie();

	// // 放平砍技能.
	void NormalAttack(const FName& InKey);

	// 覆盖ISimpleComboInterface::ComboAttack
	// 本质上执行战斗组件放出平砍GA.
	virtual void ComboAttack(const FName& InKey) override;

	// 拿取当前人物身份类型(用以敌我识别)
	ECharacterType GetCharacterType();

	// 移除死亡后遗体
	void RemoveDeadBody(float InTime = 4.f);
public:
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
	// 拿取GAS属性集.
	FORCEINLINE UMMOARPGAttributeSet* GetAttribute() { return AttributeSet; }
	// 拿取死亡动画序列号.
	FORCEINLINE int32 GetDieIndex() { return DieIndex; }

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

	// RPC至客户端, 让客户端播放伤害字体.
	UFUNCTION(Client, Reliable)
		virtual void SpawnDrawTextInClient(float InDamageAmount, const FVector& InLocation, float InRate);

public:/// 技能相关
	// 覆盖基类; 获取连招检测器.
	virtual struct FSimpleComboCheck* GetSimpleComboInfo() override;

	// 广播 刷新最新的人物GAS属性集.
	UFUNCTION(NetMulticast, Reliable)
		void UpdateCharacterAttribute(const FMMOARPGCharacterAttribute& CharacterAttribute);
	

	// 用1行DTR属性 注册更新AttributeSet指针数据
	void UpdateAttribute(const FCharacterAttributeTable* InDTRowAttribute);

	// 用1个GAS属性集 注册更新AttributeSet指针数据
	void UpdateAttribute(const FMMOARPGCharacterAttribute* InGASAttribute);

	// 处理人的血量; 虚方法
	virtual void HandleHealth(const struct FGameplayTagContainer& InTags, float InNewValue);
	// 处理人的蓝量; 虚方法
	virtual void HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue);
	// 处理人的伤害值; 虚方法
	virtual void HandleDamage(
		float DamageAmount,// 伤害值
		const struct FGameplayTagContainer& DamageTags,// 标签
		AMMOARPGCharacterBase* InstigatorPawn,// 施法者
		AActor* DamageCauser// 源ASC内的源actor
	);

	// 写入战斗组件里的受击ID
	void SetHitID(int32 InNewID);
	// 读取战斗组件里的受击ID
	const int32 GetHitID() const;

	// 执行受击
	virtual void PlayHit();
	
	// 执行死亡
	virtual void PlayDie();

	// 使用战斗组件里的 注册各部分技能(按形式来源)
	void RegisterGameplayAbility(const TArray<FName>& InGANames/*一组技能名*/, EMMOARPGGameplayAbilityType InGASrcEnum/*技能形式来源*/);

	// "单机非广播版" 用一组GA去注册1个连招黑盒
	void RegisterComboAttack(const TArray<FName>& InGANames);

	// 广播 "用一组GA注册连招黑盒"
	void RegisterComboAttackMulticast(const TArray<FName>& InGANames);

/// //////////////////////////////////////////////////////////////////////////
protected:
	// 人物动作状态.
	UPROPERTY(ReplicatedUsing = OnRep_ActionStateChanged)
		ECharacterActionState ActionState;

	// 最后一次切换到的人物动作状态.
	UPROPERTY()
		ECharacterActionState LastActionState;

	// 游玩人物专属ID.哪个角色?
	UPROPERTY(EditDefaultsOnly, Category = "Character")
		int32 ID;
	// 用户ID.用户去配置的ID.
	UPROPERTY()
		int32 UserID;

	// 死亡动画序列号.
	UPROPERTY()
		int32 DieIndex;

	// 关联动画蒙太奇DT的某 行数据.
	FCharacterAnimTable* AnimTable;
};

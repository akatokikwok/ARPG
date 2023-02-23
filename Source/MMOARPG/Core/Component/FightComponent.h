// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../Game/Abilities/MMOARPGAbilitySystemComponent.h"
#include "SimpleComboType.h"
#include "../Game/Abilities/MMOARPGGameplayAbility.h"
#include "../../MMOARPGGameType.h"
#include "FightComponent.generated.h"

/**
 * 技能插槽的数据结构
 */
USTRUCT(BlueprintType)
struct MMOARPG_API FMMOARPGSkillSlot
{
	GENERATED_USTRUCT_BODY()
public:
	// 技能名称
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		FName SkillName;// 技能名称

	// 希望释放出去的技能实例句柄
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		FGameplayAbilitySpecHandle Handle;// 希望释放出去的技能实例句柄

public:
	// 技能名称是否有意义
	bool IsVaild() const { return SkillName != NAME_None; }
	//
	void Reset();
};


/**
 * 战斗组件.继承自MotionComp
 */
UCLASS()
class MMOARPG_API UFightComponent : public UMotionComponent
{
	GENERATED_BODY()
public:
	UFightComponent();
	virtual void BeginPlay() override;

protected:
	// 从GA句柄池子里查匹配的技能
	UMMOARPGGameplayAbility* GetGameplayAbility(const FName& InKey, const TMap<FName, FGameplayAbilitySpecHandle>& InMap);

	// 从Skills 池子里找匹配的技能
	UMMOARPGGameplayAbility* GetGameplayAbilityForSkills(const FName& InKey);

	// 从ComboAttacks 池子里找匹配的技能
	UMMOARPGGameplayAbility* GetGameplayAbilityForCombos(const FName& InKey);

public:
	// 往Skill池子里写入 从DTRow里查出来的指定名字的形式攻击.
	void AddMMOARPGGameplayAbility_ToSkillpool(const FName& InKey_GAName, EMMOARPGGameplayAbilityType GAType = EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);

protected:
	/** 用指定GA去注册连招触发器黑盒. */
	void RegisterComboAttack(const FName& InGAName);

	/** 连击黑盒卸除; 黑盒数组里移除指定 combo技能的黑盒 */
	void UnregisterComboAttack(const FName& Key);

	// 添加并授权某技能. 返回技能实例的句柄.
	FGameplayAbilitySpecHandle AddAbility(TSubclassOf<UGameplayAbility> InNewAbility);

public:
	// 从一组连击黑盒检测器(空中, 地面)里按技能名获取对应的黑盒检测器
	FSimpleComboCheck* GetSimpleComboInfo(const FName& InGAkey);

	/** 连招黑盒检测器 激发; 需要1个SkillSlot的键位号 */
	void Press(int32 InSlot);

	/** Combo黑盒检测器 中止 */
	void Released(int32 InSlotKeyNumber);

	/** Combo黑盒检测器 复位 */
	void Reset();

public:
	// 放闪避技能./* 以技能页或背包里配置的技能槽内的技能为准.*/
	void DodgeSkill();

	// 放冲刺技能/* 以技能页或背包里配置的技能槽内的技能为准.*/
	void SprintSkill();

	// 激活 受击技能
	UFUNCTION(BlueprintCallable)
		void Hit();

	// 激活 死亡技能
	UFUNCTION(BlueprintCallable)
		void Die();

public:
	// 注册各部分技能(按形式来源)
	void RegisterGameplayAbility(const TArray<FName>& InGANames/*一组技能名*/, EMMOARPGGameplayAbilityType InGASrcEnum/*技能形式来源*/);

	// 核验一组连招黑盒检测器内是否有匹配的combo技能
	bool ComboChecksContains(const FName& InGAName);

	// "单机版" 用一组GA去注册1个连招黑盒
	void RegisterComboAttack(const TArray<FName>& InGANames);

public:
	// 当血量变化时候处理
	virtual void HandleHealth(AMMOARPGCharacterBase* InstigatorPawn, AActor* DamageCauser, const struct FGameplayTagContainer& InTags, float InNewValue, bool bPlayHit = true);

	// 当蓝量变化时候处理
	virtual void HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue);

	// 当经验值变化的时处理
	virtual void HandleExp(const struct FGameplayTagContainer& InTags, float InNewValue);

	// 击杀授予的奖励buff结算.
	virtual void RewardEffect(float InNewLevel, TSubclassOf<UGameplayEffect> InNewReward, TFunction<void()> InFun_AppendLogic);

	// 指明一个Pawn, 为其升等级.
	void UpdateLevel(AMMOARPGCharacterBase* InUpgradeLevelPawn);

	// 升自己等级.
	void UpdateLevel(float InLevel, TSubclassOf<UGameplayEffect> InNewReward);

public:
	// 从技能缓存池里提出所有技能名字
	void GetSkillTagsName(TArray<FName>& OutNames);

	// 从连招缓存池里提出所有连招名字
	void GetComboAttackTagsName(TArray<FName>& OutNames);

	// 从肢体缓存池里提出所有肢体动作名字
	void GetLimbsTagsName(TArray<FName>& OutNames);

public:
	/** 核心接口; 从某种GA缓存池里提出给定名字的GA并激活它, 可能会激活失败. */
	bool TryActivateAbility(const FName& InTagName, const TMap<FName, FGameplayAbilitySpecHandle>& InMap);

	// 暂停使用, 旧接口
	UFUNCTION(BlueprintCallable)
		bool Combo(const FName& InKey);

	// 释放Combo来源型技能,从Combos缓存池里, 会判断释放是否成功
	UFUNCTION(BlueprintCallable)
		bool Attack_Combo(const FName& InKey);// 放GA: 普攻.

	// 释放Skill来源型技能,从Skills缓存池里, 会判断释放是否成功
	UFUNCTION(BlueprintCallable)
		bool Skill(const FName& InKey);

	// 释放Limb来源型技能,从Limbs缓存池里, 会判断释放是否成功
	UFUNCTION(BlueprintCallable)
		bool Limb(const FName& InKey);

public:
	// 让指定键号的技能槽放出对应的Skill型(非COMBO/LIMB)技能.
	bool SKillAttack(int32 InSlotKeyNumber);

	/** 往Skill槽容器注册1份 技能槽数据 (并判断操作是否成功) */
	bool AddSkillSlot(int32 InSlot, const FMMOARPGSkillSlot& InSkillSlot);

	/** 用入参组一份技能槽数据并注册至Skill槽容器 (并判断操作是否成功) */
	bool AddSkillSlot(int32 InSlot, const FName& InSkillNameTag);

	/** 移除SkillTMap的技能节点并查询是否成功 */
	bool RemoveSkillSlot(int32 InSlot);

	/** 移除技能并查询是否成功 */
	bool SwapSkillSlot(int32 InASlot, int32 InBSlot);

	/** 移动技能并查询是否成功 */
	bool MoveSkillSlot(int32 InASlot, int32 InBSlot);
public:
	/** Skills池子里注册元素(元素来之人物DT技能表) */
	FGameplayAbilitySpecHandle AddSkill(const FName& InNameTag);

	/** ComboAttacks池子里注册元素(元素来自人物DT技能表) */
	FGameplayAbilitySpecHandle AddComboAttacks(const FName& InTags);

	/** 在横框, 移除指定槽号的旧技能并添加新技能 */
	bool RemoveSkillSlot(int32 InSlot, const FName& InSkillName);

	/** 移除 ComboAttacks这个池子里的某个技能 */
	void RemoveComboAttacks(const FName& InTags);

protected:
	/** 从总缓存池内移除指定TagName的技能 */
	void RemoveSkill(const FName& InNameTag);

	/** 小接口: ASC移除给定句柄的技能 */
	void ClearAbility(FGameplayAbilitySpecHandle InHanle);

public:
	// 初始化技能
	void InitSkill();

	// 更新表
	void UpdateSkillTable();

	// 更新技能节点
	void UpdateSkillSlots();

public:
	// 反序列化 已处理好的技能bit并最终处理其真实句柄
	void DeserializationSkillAssembly(const FString& InString);
	// 序列化后输出一个 为带顺序的bit位的技能名字
	void SerializationSkillAssembly(FString& OutString);

public:
	// 激活持续恢复buff
	void ActivateRecoveryEffect(TSubclassOf<UGameplayEffect> InGameplayEffect);

	// 解除持续恢复buff
	void DeactivationRecoveryEffect(TSubclassOf<UGameplayEffect> InGameplayEffect);

protected:
	// 提出Skills池子里活跃标签的GA
	UMMOARPGGameplayAbility* GetGameplayAbilityActiveTagBySkill();

public:
	// 给键位号, 去技能槽池子里找匹配的元素
	FMMOARPGSkillSlot* FindSkillSlot(int32 InSlotKeyNum);

	/** 槽位的活跃标签数据验证 ,需要1个技能槽键位号 */
	bool CheckConditionSKill(int32 InSlot);

	// 通过标签添加闪避诱发的霸体效果BUFF
	void ApplyDodgeEffect();

	// 按技能来源分型(Skill/Combo/Limb),激活指定名字GA
	virtual void ExecuteGameplayAbility(EMMOARPGGameplayAbilityType InGameplayAbilityType, const FName& InName);

	// 让指定GE BUFF效果应用至自身ASC
	virtual void ExecuteGameplayEffect(const TSubclassOf<UGameplayEffect>& InGameplayEffect);

	//////////////////////////////////////////////////////////////////////////

private:
	/**来自人物基类的ASC
	 * 战斗组件也持有1个ASC
	 * 弱指针,需要检查是否被销毁.
	 */
	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TWeakObjectPtr<UMMOARPGAbilitySystemComponent> AbilitySystemComponent;

	// 一组Combo形式的攻击 黑盒触发器(包含空中连击, 地面连击)
	UPROPERTY()
		TArray<FSimpleComboCheck> ComboAttackChecks;

public:
	// 受击ID
	UPROPERTY()
		int32 HitID;// 受击ID

	// 本角色是否被挑飞
	UPROPERTY()
		bool bPickFly = false;

protected:
	// 技能(如冲刺,躲闪)缓存池,  1个名字对应1个GA句柄
	TMap<FName, FGameplayAbilitySpecHandle> Skills;
	// 连招缓存池
	TMap<FName, FGameplayAbilitySpecHandle> ComboAttacks;
	// 肢体行为缓存池
	TMap<FName, FGameplayAbilitySpecHandle> Limbs;

protected:
	/** 可视化的插槽技能释放表,只有存在着里面才被授权使用 */
	TMap<int32, FMMOARPGSkillSlot> SkillSlotsTMap;
};

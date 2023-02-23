// Fill out your copyright notice in the Description page of Project Settings.


#include "FightComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ThreadManage.h"
#include "Net/UnrealNetwork.h"
#include "../Game/MMOARPGGameState.h"
#include "../Game/Abilities/MMOARPGGameplayAbility.h"
#include "../Game/Character/MMOARPGCharacter.h"

// 由于MMOARPGCharacter里已经包含了MMOTagList.h, 所以直接extern
extern bool SingleBitAndOrderToTagName(const int32 InEnumIndex, const uint64 EnumValue, FName& TagName);
extern bool SingleTagNameToBitAndOrder(const FName& TageName, int32& InEnumIndex, uint64& EnumValue);

UFightComponent::UFightComponent()
{
	// 由于技能有类型细分,1~5是通用技能
	for (int32 i = 0; i < (((int32)EMMOARPGSkillType::MAX_SKILLS) - 2); i++) {
		SkillSlotsTMap.Add(i + 1, FMMOARPGSkillSlot());
	}
}

void UFightComponent::BeginPlay()
{
	Super::BeginPlay();

	// 初始化 持有本组件的人物基类.
	MMOARPGCharacterBase = Cast<AMMOARPGCharacterBase>(GetOwner());

	if (MMOARPGCharacterBase.IsValid()) {
		// 初始化 人里的ASC.	
		AbilitySystemComponent = Cast<UMMOARPGAbilitySystemComponent>(MMOARPGCharacterBase->GetAbilitySystemComponent());

		// 仅允许服务器注册ASC的持有对象(即人物基类.).
		if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority) {
			AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
		}
	}
}

// 添加并授权某技能. 返回技能实例的句柄.
FGameplayAbilitySpecHandle UFightComponent::AddAbility(TSubclassOf<UGameplayAbility> InNewAbility)
{
	if (IsValid(InNewAbility) && AbilitySystemComponent.IsValid()) {
		return AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(InNewAbility));
	}

	return FGameplayAbilitySpecHandle();
}

// 从GA句柄池子里查匹配的技能
UMMOARPGGameplayAbility* UFightComponent::GetGameplayAbility(const FName& InKey, const TMap<FName, FGameplayAbilitySpecHandle>& InSpecMap)
{
	if (const FGameplayAbilitySpecHandle* InHandle = InSpecMap.Find(InKey)) {
		if (AbilitySystemComponent.IsValid()) {
			if (FGameplayAbilitySpec* ItsSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(*InHandle)) {
				return Cast<UMMOARPGGameplayAbility>(ItsSpec->Ability);
			}
		}
	}
	return nullptr;
}

// 从Skills 池子里找匹配的技能
UMMOARPGGameplayAbility* UFightComponent::GetGameplayAbilityForSkills(const FName& InKey)
{
	return GetGameplayAbility(InKey, Skills);
}

// 从ComboAttacks 池子里找匹配的技能
UMMOARPGGameplayAbility* UFightComponent::GetGameplayAbilityForCombos(const FName& InKey)
{
	return GetGameplayAbility(InKey, ComboAttacks);
}

// 放闪避技能./* 以技能页或背包里配置的技能槽内的技能为准.*/
void UFightComponent::DodgeSkill/*_Implementation*/()
{
	FName CurrentSkillName = SkillSlotsTMap[(int32)EMMOARPGSkillType::DODGE_SKILL].SkillName;// 从技能槽池子取出匹配的技能
	if (!CurrentSkillName.IsNone()) {
		Skill(CurrentSkillName);
	}
}

// 放冲刺技能/* 以技能页或背包里配置的技能槽内的技能为准.*/
void UFightComponent::SprintSkill/*_Implementation*/()
{
	FName CurrentSkillName = SkillSlotsTMap[(int32)EMMOARPGSkillType::SPRINT_SKILLS].SkillName;// 从技能槽池子取出匹配的技能
	if (!CurrentSkillName.IsNone()) {
		Skill(CurrentSkillName);
	}
}

// 激活 受击技能
void UFightComponent::Hit()
{
	if (AbilitySystemComponent.IsValid()) {
		static const FName Player_State_Hit_Name(TEXT("Player.State.Hit"));
		Limb(Player_State_Hit_Name);// 从肢体缓存池里激活名为Player.State.Hit的受击GA
	}
}

// 激活 死亡技能
void UFightComponent::Die()
{
	if (AbilitySystemComponent.IsValid()) {
		static const FName Player_State_Die_Name(TEXT("Player.State.Die"));
		Limb(Player_State_Die_Name);// 从肢体缓存池里激活名为Player.State.Die的死亡GA
	}
}

/** 往Skills大池子里注册写入  指定Tag的技能. */
void UFightComponent::AddMMOARPGGameplayAbility_ToSkillpool(const FName& InKey_GAName, EMMOARPGGameplayAbilityType GAType /*= EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK*/)
{
	if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {
		// 用GameState找出人身上配的 DTRow::技能表
		if (FCharacterSkillTable* InSkillTable_row = InGS->GetCharacterSkillTable(InKey_GAName, MMOARPGCharacterBase->GetID())) {
			/* 按技能形式来源切分, 分三类*/
			switch (GAType) {
				case EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK:
					Skills.Add(InKey_GAName, AddAbility(InSkillTable_row->GameplayAbility));// 为skill池子添加元素
					break;
				case EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK:
					ComboAttacks.Add(InKey_GAName, AddAbility(InSkillTable_row->GameplayAbility));// 为连招池子添加元素
					break;
				case EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_LIMBS:
					Limbs.Add(InKey_GAName, AddAbility(InSkillTable_row->GameplayAbility));// 为肢体池子添加元素
					break;
			}
		}
	}
}

// 从一组连击黑盒检测器(空中, 地面)里按技能名获取对应的黑盒检测器
FSimpleComboCheck* UFightComponent::GetSimpleComboInfo(const FName& InGAkey)
{
	return ComboAttackChecks.FindByPredicate(
		[InGAkey](const FSimpleComboCheck& InComboCheck) ->bool {
			return InComboCheck.ComboKey_GA == InGAkey;
		});
}

/** 连招黑盒检测器 激发; 需要1个SkillSlot的键位号 */
void UFightComponent::Press(int32 InSlot)
{
	FName CurrentSKillComboName = SkillSlotsTMap[InSlot].SkillName;// 先取出匹配的技能槽里的技能

	for (FSimpleComboCheck& AnyComboCheck : ComboAttackChecks) {
		if (UMotionComponent::IsAir()) {// 在空中
			if (AnyComboCheck.ComboKey_GA == CurrentSKillComboName /*TEXT("Player.Attack.ComboLinkage.Air")*/) {// 放的技能是空中连击;// 必须匹配对应的COMBO,分空中和地面型
				AnyComboCheck.Press();

				// 测试代码, 非正式, 调镜头操作
				if (AMMOARPGCharacter* InChar = Cast<AMMOARPGCharacter>(MMOARPGCharacterBase)) {
					InChar->HandleCameraViewWhenAirCombo();
				}
				break;
			}
		}
		else {// 在地面
			if (AnyComboCheck.ComboKey_GA == CurrentSKillComboName /*TEXT("Player.Attack.ComboLinkage.Ground")*/) {// 放的是地面连击// 必须匹配对应的COMBO,分空中和地面型
				AnyComboCheck.Press();
				break;
			}
		}
	}
}

/** Combo黑盒检测器 中止 */
void UFightComponent::Released(int32 InSlotKeyNumber)
{
	FName CurrentSKillComboName = SkillSlotsTMap[InSlotKeyNumber].SkillName;// 先取出匹配的技能槽里的技能

	for (FSimpleComboCheck& AnyComboCheck : ComboAttackChecks) {
		if (AnyComboCheck.ComboKey_GA == CurrentSKillComboName) {// 必须匹配对应的COMBO,分空中和地面型
			AnyComboCheck.Released();
			break;
		}

		// 测试代码, 非正式, 调镜头操作
		if (UMotionComponent::IsAir()) {// 在空中
			AnyComboCheck.Released();
			if (AMMOARPGCharacter* InChar = Cast<AMMOARPGCharacter>(MMOARPGCharacterBase)) {
				InChar->HandleCameraViewWhenNotInAirCombo();
			}
		}
	}
}

/** Combo黑盒检测器 复位 */
void UFightComponent::Reset()
{
	for (FSimpleComboCheck& AnyComboCheck : ComboAttackChecks) {
		AnyComboCheck.Reset();
	}
}

// 注册各部分技能(按形式来源)
void UFightComponent::RegisterGameplayAbility(const TArray<FName>& InGANames, EMMOARPGGameplayAbilityType InGASrcEnum)
{
	// 核验 基类人和ASC
	if (MMOARPGCharacterBase.IsValid() && AbilitySystemComponent.IsValid()) {
		// 		 const FName InKey = TEXT("Player.Attack.ComboLinkage");

		/* 仅运行在服务器的逻辑. */
		if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority) {
			// 往Skill池子里写入 从DTRow里查出来的一组	GA
			for (auto& Tmp : InGANames) {
				AddMMOARPGGameplayAbility_ToSkillpool(Tmp, InGASrcEnum);
			}
		}
	}
}

// 核验一组连招黑盒检测器内是否有匹配的combo
bool UFightComponent::ComboChecksContains(const FName& InGAName)
{
	if (ComboAttackChecks.FindByPredicate([&](const FSimpleComboCheck& InComboChecker) {
		return InComboChecker.ComboKey_GA == InGAName;
		})) {
		return true;
	}
	return false;
}

/** 用1个GA去注册1个连招黑盒. */
void UFightComponent::RegisterComboAttack(const FName& InGAName)
{
	if (!ComboChecksContains(InGAName)) {// 若这组黑盒内不包含匹配名字的COMBO技能

		FSimpleComboCheck& InComboAttackCheck = ComboAttackChecks.AddDefaulted_GetRef();// 往连击检测容器内手动构造1个
		InComboAttackCheck.Character_CombatInterface = MMOARPGCharacterBase.Get();
		InComboAttackCheck.ComboKey_GA = InGAName;
		if (UMMOARPGGameplayAbility* GameplayAbility = GetGameplayAbilityForCombos(InGAName)) {/*先按名字从ComboAttacks池子里找GA,并把触发器的段数注册成GA里蒙太奇段数.*/
			InComboAttackCheck.MaxIndex = GameplayAbility->GetCompositeSectionsNumber();
		}
		else {/*没找到GA就给个4段. */
			InComboAttackCheck.MaxIndex = 4;
		}
	}
}

// 用一组GA去注册1个连招黑盒
void UFightComponent::RegisterComboAttack(const TArray<FName>& InGANames)
{
	for (auto& GATag : InGANames) {
		RegisterComboAttack(GATag);
	}
}

/** 连击黑盒卸除; 黑盒数组里移除指定 combo技能的黑盒 */
void UFightComponent::UnregisterComboAttack(const FName& Key)
{
	if (ComboChecksContains(Key)) {// 核验一组连招黑盒检测器内包含有匹配的combo技能
		FSimpleComboCheck CheckerNeedToRemove;
		CheckerNeedToRemove.ComboKey_GA = Key;

		/* 因为黑盒内重载了== 操作符, 所以可以支持真正意义上的移除*/
		ComboAttackChecks.Remove(CheckerNeedToRemove);// 黑盒数组移除指定元素
	}
}

// 当血量变化时候处理
void UFightComponent::HandleHealth(AMMOARPGCharacterBase* InstigatorPawn, AActor* DamageCauser, const struct FGameplayTagContainer& InTags, float InNewValue, bool bPlayHit)
{
	if (MMOARPGCharacterBase.IsValid()) {
		// 目标任务是否死亡(死亡后会执行奖励buff结算)
		if (MMOARPGCharacterBase->IsDie()) {
			/* 对施法者 进行击杀战利结算贡献.种类是 "击杀奖励buff".*/
			InstigatorPawn->RewardEffect(MMOARPGCharacterBase->GetCharacterLevel(),
				MMOARPGCharacterBase->GetDeathRewardEffect(),
				[&]() ->void {
					// 施法者是否满足了升等级条件. 满足则给一份新的递归式的buff奖励, 种类是 "施法者提升经验值buff".
					this->UpdateLevel(InstigatorPawn);
				}
			);

			/* 对目标人物 执行使其死亡 最后执行.*/
			MMOARPGCharacterBase->PlayDie();

			// 只有玩家角色才可以创建重生提示弹窗(仅在客户端)
			if (AMMOARPGCharacter* InCharacter = Cast<AMMOARPGCharacter>(MMOARPGCharacterBase)) {
				InCharacter->CreateResurrectionWindowsClient();
			}
		}
		else {
			// 施法者和受击序号必须有意义.
			if (InstigatorPawn != nullptr) {
				if (MMOARPGCharacterBase->GetHitID() != INDEX_NONE) {
					// 让挨打的人面朝向 施法攻击者.
					FRotator TargetRot = (-InstigatorPawn->GetActorForwardVector()).ToOrientationRotator();
					MMOARPGCharacterBase->SetActorRotation(TargetRot);

					/* 这里特殊处理一下一种情形, 即施法者在滞空下坠时候对地面状态的敌人进行空中连击 */
					if (MMOARPGCharacterBase->GetHitID() == 6) {// 测试用,暂时认为受击ID是6的时候 施法者跳在空中使用空中Combo,敌人受到为6的空中受击
						if (!IsAir()) {// 但施法者缺正在从滞空坠落到地面
							MMOARPGCharacterBase->SetHitID(FMath::RandRange(0, 1));// 人跌落到地面后再受击, 暂时重置置敌人受击ID为0或1
						}
					}

					// 对目标人物, 再执行让它挨打受击.
					if (bPlayHit) {
						MMOARPGCharacterBase->PlayHit();
					}
				}
			}
		}
	}
}

// 当蓝量变化时候处理
void UFightComponent::HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue)
{

}

void UFightComponent::HandleExp(const struct FGameplayTagContainer& InTags, float InNewValue)
{

}

void UFightComponent::RewardEffect(float InNewLevel, TSubclassOf<UGameplayEffect> InNewRewardBuff, TFunction<void()> InFun_AppendLogic)
{
	if (AbilitySystemComponent.IsValid()) {
		// 给自身应用 奖励机制GE
		AbilitySystemComponent->ApplyGameplayEffectToSelf(Cast<UGameplayEffect>(InNewRewardBuff->GetDefaultObject()),
			InNewLevel,
			AbilitySystemComponent->MakeEffectContext()
		);

		// 执行传进来的附加逻辑段.
		InFun_AppendLogic();
	}
}

// 指明一个Pawn, 为其升等级.
void UFightComponent::UpdateLevel(AMMOARPGCharacterBase* InUpgradeLevelPawn)
{
	if (InUpgradeLevelPawn->IsUpdateLevel()) {
		// 让人升一级
		InUpgradeLevelPawn->UpdateLevel(InUpgradeLevelPawn->GetCharacterLevel() + 1);

		// 更新角色的技能Slot
		if (AMMOARPGCharacter* InCharacter = Cast<AMMOARPGCharacter>(InUpgradeLevelPawn)) {
			InCharacter->UpdateSkillSlots();
		}

		// 递归判定
		this->UpdateLevel(InUpgradeLevelPawn);
	}
}

// 升自己等级.
void UFightComponent::UpdateLevel(float InLevel, TSubclassOf<UGameplayEffect> InNewReward)
{
	if (AbilitySystemComponent.IsValid()) {
		// 断言警告提示, 若奖励GE未配置则会提醒开发者.
		checkf(InNewReward, TEXT("这张奖励机制的GE 必须在蓝图中被配置."));

		// 给自身应用 奖励机制GE
		AbilitySystemComponent->ApplyGameplayEffectToSelf(Cast<UGameplayEffect>(InNewReward->GetDefaultObject()),
			InLevel,
			AbilitySystemComponent->MakeEffectContext()
		);
	}
}

// 从技能缓存池里提出所有技能名字
void UFightComponent::GetSkillTagsName(TArray<FName>& OutNames)
{
	Skills.GetKeys(OutNames);
}

// 从连招缓存池里提出所有连招名字
void UFightComponent::GetComboAttackTagsName(TArray<FName>& OutNames)
{
	ComboAttacks.GetKeys(OutNames);
}

// 从肢体缓存池里提出所有肢体动作名字
void UFightComponent::GetLimbsTagsName(TArray<FName>& OutNames)
{
	Limbs.GetKeys(OutNames);
}

void FMMOARPGSkillSlot::Reset()
{
	*this = FMMOARPGSkillSlot();
}

#pragma region 关于真正GA池子的一些操作函数
/** Skills池子里注册元素(元素来之人物DT技能表) */
FGameplayAbilitySpecHandle UFightComponent::AddSkill(const FName& InNameTag)
{
	if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
		if (FCharacterSkillTable* InSkillTable = InGameState->GetCharacterSkillTable(InNameTag, MMOARPGCharacterBase->GetID())) {// 从DT里读指定游玩人物专属ID的 一行技能信息
			if (!Skills.Contains(InNameTag)) {
				Skills.Add(InNameTag, this->AddAbility(InSkillTable->GameplayAbility));
				return Skills[InNameTag];// 并返回这个技能句柄
			}
		}
	}
	return FGameplayAbilitySpecHandle();// 否则返回空技能句柄
}

/** ComboAttacks池子里注册元素(元素来自人物DT技能表) */
FGameplayAbilitySpecHandle UFightComponent::AddComboAttacks(const FName& InTags)
{
	if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
		if (FCharacterSkillTable* InSkillTable = InGameState->GetCharacterSkillTable(InTags, MMOARPGCharacterBase->GetID())) {
			if (!ComboAttacks.Contains(InTags)) {
				ComboAttacks.Add(InTags, AddAbility(InSkillTable->GameplayAbility));
				return ComboAttacks[InTags];
			}
		}
	}
	return FGameplayAbilitySpecHandle();
}

/** 从总缓存池内移除指定TagName的技能 */
void UFightComponent::RemoveSkill(const FName& InNameTag)
{
	if (Skills.Contains(InNameTag)) {
		//  小接口: ASC移除给定句柄的技能
		this->ClearAbility(Skills[InNameTag]);
		// 移除"Skills容器" 里的一组pair
		Skills.Remove(InNameTag);
	}
}

/** 移除 ComboAttacks这个池子里的某个技能 */
void UFightComponent::RemoveComboAttacks(const FName& InGAName)
{
	if (ComboAttacks.Contains(InGAName)) {
		ClearAbility(ComboAttacks[InGAName]);

		//移除我们的Map
		ComboAttacks.Remove(InGAName);
	}
}

/** 小接口: ASC移除给定句柄的技能 */
void UFightComponent::ClearAbility(FGameplayAbilitySpecHandle InHanle)
{
	AbilitySystemComponent->ClearAbility(InHanle);
}

void UFightComponent::InitSkill()
{
	// 更新不同的技能表(SkillPage)
	this->UpdateSkillTable();
	// 更新不同的技能节点(SkillSlots)
	this->UpdateSkillSlots();
}

// 更新表(SkillPage)
void UFightComponent::UpdateSkillTable()
{
	if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
		if (AMMOARPGCharacter* InCharacter = Cast<AMMOARPGCharacter>(MMOARPGCharacterBase)) {

			// 0. 待加工的一组技能表技能或是横框内技能
			TArray<FName> ResultSkillTags;

			// I. 依次从三种技能形式来源的池子里 提取出Skill型,Combo型,Limb型的技能名字
			TArray<FName> SkillTags;
			GetSkillTagsName(SkillTags);

			TArray<FName> ComboAttackTags;
			GetComboAttackTagsName(ComboAttackTags);

			TArray<FName> LimbsTags;
			GetLimbsTagsName(LimbsTags);

			// II. 取出本人物的所有技能Table行
			TArray<FCharacterSkillTable*> OutSKillTableRows;
			InGameState->GetCharacterSkillsTables(InCharacter->GetID(), OutSKillTableRows);

			/** Lambda: 给一组技能名,看看给定的技能名是否位于其中 */
			auto FindGameplayTags = [&](const FString& InString, const TArray<FName>& InSkillNames)->bool {
				for (const FName& SkillName : InSkillNames) {
					if (InString == SkillName.ToString()) {
						return true;
					}
				}
				return false;
			};

			// III. 给ResultTags做值; 找到技能行里没有的技能,有差异的地方
			for (auto& SKillTableRow : OutSKillTableRows) {
				check(SKillTableRow->GameplayAbility);// 断言TableRow里的技能蓝图

				if (UGameplayAbility* InGameplayAbility = Cast<UGameplayAbility>(SKillTableRow->GameplayAbility->GetDefaultObject())) {// TableRow里的具体技能

					bool bExist = false;

					// 真实技能的AbilityTags.; FName不可靠, 使用ToStringSimple提取出真实技能的AbilityTags.
					const FString TagString = InGameplayAbility->AbilityTags.ToStringSimple();

					// 若发现真实技能Tag都不被这三种类型包含, 则手动给技能表填充元素
					if (!FindGameplayTags(TagString, SkillTags) &&
						!FindGameplayTags(TagString, LimbsTags) &&
						!FindGameplayTags(TagString, ComboAttackTags)) {
						ResultSkillTags.Add(*TagString);
					}
				}
			}

			// IV. 在客户端 更新技能表(SkillPage)-仅UI外观
			InCharacter->UpdateSkillTableOnClient(ResultSkillTags);
		}
	}
}

// 更新技能节点(SkillSlots)
void UFightComponent::UpdateSkillSlots()
{
	if (AMMOARPGCharacter* InCharacter = Cast<AMMOARPGCharacter>(MMOARPGCharacterBase)) {
		TArray<FName> InSkillTags;
		for (auto& Itr : SkillSlotsTMap) {
			InSkillTags.Add(Itr.Value.SkillName);// 提取出所有的技能名字存储1个数组
		}

		// 在客户端 更新技能槽节点(横框)-仅UI外观
		InCharacter->UpdateSkillSlotsOnClient(InSkillTags);
	}
}

void UFightComponent::DeserializationSkillAssembly(const FString& InString)
{
	// 用逗号间隔切碎输入的字符串
	TArray<FString> MapArrays;
	InString.ParseIntoArray(MapArrays, TEXT(","));

	if (SkillSlotsTMap.Num()) {
		for (int32 i = 0; i < MapArrays.Num(); i++) {
			int32 Key;
			int32 EnumIndex = INDEX_NONE;
			uint64 TagNameBit = 0;
			{
				FString K, V;
				MapArrays[i].Split(TEXT(":"), &K, &V);
				Key = FCString::Atoi(*K);

				FString E, B;
				V.Split(TEXT("|"), &E, &B);
				EnumIndex = FCString::Atoi(*E);

				TagNameBit = FCString::Strtoui64(*B, NULL, 10);
			}

			SingleBitAndOrderToTagName(EnumIndex, TagNameBit, SkillSlotsTMap[Key].SkillName);

			// 对Skills池子和Combos池子 分别查找匹配的技能槽键位号,获取其GA句柄spec
			if (FGameplayAbilitySpecHandle* InSkillHandle = Skills.Find(SkillSlotsTMap[Key].SkillName)) {
				SkillSlotsTMap[Key].Handle = *InSkillHandle;
			}
			else if (FGameplayAbilitySpecHandle* InComboHandle = ComboAttacks.Find(SkillSlotsTMap[Key].SkillName)) {
				SkillSlotsTMap[Key].Handle = *InComboHandle;
			}
		}
	}
}

void UFightComponent::SerializationSkillAssembly(FString& OutString)
{
	for (auto& Tmp : SkillSlotsTMap) {// 扫描插槽map容器
		uint64 Bit = 0llu; // 无符号64位
		int32 EnumIndex = INDEX_NONE;
		// 把给定的技能名字转换为带顺序的bit位
		SingleTagNameToBitAndOrder(Tmp.Value.SkillName, EnumIndex, Bit);

		OutString += FString::Printf(TEXT("%i:%i|%llu,"), Tmp.Key, EnumIndex, Bit);
	}
	// 多余的拼法逗号删掉.
	OutString.RemoveFromEnd(TEXT(","));
}

void UFightComponent::ActivateRecoveryEffect(TSubclassOf<UGameplayEffect> InGameplayEffect)
{
	if (GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
		if (MMOARPGCharacterBase.IsValid()) {
			// 1.先填充GE句柄context
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(UMotionComponent::MMOARPGCharacterBase.Get());

			// 2.用入参构建1个GE实例并应用
			FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InGameplayEffect,
				MMOARPGCharacterBase->GetCharacterLevel(), EffectContext);
			if (EffectSpecHandle.IsValid()) {
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), AbilitySystemComponent.Get());
			}
		}
	}
}

void UFightComponent::DeactivationRecoveryEffect(TSubclassOf<UGameplayEffect> InGameplayEffect)
{
	if (GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(InGameplayEffect, AbilitySystemComponent.Get());
	}
}

#pragma endregion 关于真正GA池子的一些操作函数

//////////////////////////////////////////////////////////////////////////

#pragma region 核心内部接口; 可用来激活底层的三个句柄池子里的GA

/** 核心接口; 从某种GA缓存池里提出给定名字的GA并激活它, 可能会激活失败. */
bool UFightComponent::TryActivateAbility(const FName& InTagName, const TMap<FName, FGameplayAbilitySpecHandle>& InMap)
{
	// 检查弱指针ASC是否被破坏.
	if (AbilitySystemComponent.IsValid()) {
		if (const FGameplayAbilitySpecHandle* Handle = InMap.Find(InTagName)) {
			AbilitySystemComponent->TryActivateAbility(*Handle);
			return true;
		}
	}
	return false;
}

// 旧接口, 暂停使用
bool UFightComponent::Combo(const FName& InKey)
{
	//...
	return false;
}

// 释放Combo来源型技能,从Combos缓存池里, 会判断释放是否成功
bool UFightComponent::Attack_Combo(const FName& InGAName)
{
	if (MMOARPGCharacterBase->IsA(AMMOARPGCharacter::StaticClass())) {// 强制只承认AMMOARPGCharacter型的pawn
		/* 排除一些不适用的空中连击情形,这些情形会释放GA失败*/
		FName CurrentSKillName = SkillSlotsTMap[(int32)EMMOARPGSkillType::COMBO_AIR_SKILL].SkillName;
		if (InGAName == CurrentSKillName) {
			if (!UMotionComponent::IsAir()) {
				return false;
			}
		}
	}
	/* 其余大多数情况,直接释放COMBO连击GA. */
	return TryActivateAbility(InGAName, ComboAttacks);
}

// 释放Skill来源型技能,从Skills缓存池里, 会判断释放是否成功
bool UFightComponent::Skill(const FName& InKey)
{
	return TryActivateAbility(InKey, Skills);
}

// 释放Limb来源型技能,从Limbs缓存池里, 会判断释放是否成功
bool UFightComponent::Limb(const FName& InKey)
{
	return TryActivateAbility(InKey, Limbs);
}
#pragma endregion  核心内部接口; 可用来激活底层的三个句柄池子里的GA

//////////////////////////////////////////////////////////////////////////

#pragma region 技能槽及技能形式技能的接口
// 让指定键号的技能槽放出对应的Skill型(非COMBO/LIMB)技能.
bool UFightComponent::SKillAttack(int32 InSlotKeyNumber)
{
	if (SkillSlotsTMap.Contains(InSlotKeyNumber)) {
		if (SkillSlotsTMap[InSlotKeyNumber].IsVaild()) {// 技能名称有意义
			if (!Skill(SkillSlotsTMap[InSlotKeyNumber].SkillName)) {// 可以激活指定名字的技能
				/** 使用ASC激活技能池子里指定槽号的技能句柄 */
				return AbilitySystemComponent->TryActivateAbility(SkillSlotsTMap[InSlotKeyNumber].Handle);
			}
		}
	}
	return false;
}

/** 往SkillMap里注册1份 技能槽数据 (并判断操作是否成功) */
bool UFightComponent::AddSkillSlot(int32 InSlot, const FMMOARPGSkillSlot& InSkillSlot)
{
	// 已存在则允许覆盖
	if (SkillSlotsTMap.Contains(InSlot)) {
		SkillSlotsTMap[InSlot] = InSkillSlot;
		return true;
	}
	return false;
}

/** 往1个技能槽内存入技能数据 */
bool UFightComponent::AddSkillSlot(int32 InSlotKeyNumber, const FName& InSkillNameTag)
{
	FMMOARPGSkillSlot SkillSlot;
	EMMOARPGSkillType SkillType = (EMMOARPGSkillType)InSlotKeyNumber;

	if (SkillType == EMMOARPGSkillType::COMBO_GROUND_SKILL || SkillType == EMMOARPGSkillType::COMBO_AIR_SKILL) {
		// 把技能槽的技能实例设为ComboAttacks池子的技能实例
		SkillSlot.Handle = AddComboAttacks(InSkillNameTag);
		// 注册连击
		RegisterComboAttack(InSkillNameTag);
	}
	else {// 非COMBO分型的, 则继续添加技能
		SkillSlot.Handle = AddSkill(InSkillNameTag);
	}
	SkillSlot.SkillName = InSkillNameTag;

	// 往SkillMap里注册 刚才那份技能槽数据
	return this->AddSkillSlot(InSlotKeyNumber, SkillSlot);
}

/** 移除SkillTMap的技能节点并查询是否成功 */
bool UFightComponent::RemoveSkillSlot(int32 InSlotKeyNumber)
{
	if (SkillSlotsTMap.Contains(InSlotKeyNumber)) {
		EMMOARPGSkillType SkillType = (EMMOARPGSkillType)InSlotKeyNumber;
		if (SkillType == EMMOARPGSkillType::COMBO_GROUND_SKILL || SkillType == EMMOARPGSkillType::COMBO_AIR_SKILL) {
			/* 移除ComboAttacks这个池子里的指定键号技能.*/
			RemoveComboAttacks(SkillSlotsTMap[InSlotKeyNumber].SkillName);

			/* 连击黑盒卸除; 黑盒数组里移除指定 combo技能的黑盒 */
			UnregisterComboAttack(SkillSlotsTMap[InSlotKeyNumber].SkillName);
		}
		else {// 非Combo型, 比如是Skills型的
			RemoveSkill(SkillSlotsTMap[InSlotKeyNumber].SkillName);// 移除真实技能
		}

		// 这个特定的技能槽位再被置空
		SkillSlotsTMap[InSlotKeyNumber] = FMMOARPGSkillSlot();
		return true;
	}
	return false;
}

/** 在横框, 移除指定槽号的旧技能并添加新技能 */
bool UFightComponent::RemoveSkillSlot(int32 InRemoveSlot, const FName& InSkillName)
{
	// 移除SkillTmap的技能节点
	RemoveSkillSlot(InRemoveSlot);

	// 添加入参组的一个技能包
	return AddSkillSlot(InRemoveSlot, InSkillName);
}

/** 交换技能并查询是否成功 */
bool UFightComponent::SwapSkillSlot(int32 InASlot, int32 InBSlot)
{
	if (InASlot != InBSlot && SkillSlotsTMap.Contains(InASlot) && SkillSlotsTMap.Contains(InBSlot)) {
		if (SkillSlotsTMap[InASlot].IsVaild() || SkillSlotsTMap[InBSlot].IsVaild()) {
			FMMOARPGSkillSlot Tmp = SkillSlotsTMap[InASlot];
			SkillSlotsTMap[InASlot] = SkillSlotsTMap[InBSlot];
			SkillSlotsTMap[InBSlot] = Tmp;
			return true;
		}
	}
	return false;
}

/** 移动技能并查询是否成功 */
bool UFightComponent::MoveSkillSlot(int32 InASlot, int32 InBSlot)
{
	if (InASlot != InBSlot && SkillSlotsTMap.Contains(InASlot) && SkillSlotsTMap.Contains(InBSlot)) {
		if (SkillSlotsTMap[InASlot].IsVaild() && !SkillSlotsTMap[InBSlot].IsVaild()) {
			// B槽被A槽填充,且A槽清空复位
			SkillSlotsTMap[InBSlot] = SkillSlotsTMap[InASlot];
			SkillSlotsTMap[InASlot].Reset();
			return true;
		}
	}
	return false;
}
#pragma endregion 技能槽及技能形式技能的接口

// 提出Skills池子里活跃标签的GA
UMMOARPGGameplayAbility* UFightComponent::GetGameplayAbilityActiveTagBySkill()
{
	if (const FGameplayTagContainer* InTagContainer = AbilitySystemComponent->GetCurrentActiveSkillTags()) {
		return GetGameplayAbilityForSkills(*(InTagContainer->ToStringSimple()));
	}
	return nullptr;
}

// 给键位号, 去技能槽池子里找匹配的元素
FMMOARPGSkillSlot* UFightComponent::FindSkillSlot(int32 InSlotKeyNum)
{
	if (SkillSlotsTMap.Contains(InSlotKeyNum)) {
		return SkillSlotsTMap.Find(InSlotKeyNum);
	}
	return nullptr;
}

/** 槽位的活跃标签数据验证 ,需要1个技能槽键位号 */
bool UFightComponent::CheckConditionSKill(int32 InSlotKeyNumber)
{
	if (FMMOARPGSkillSlot* InSkillSlot = FindSkillSlot(InSlotKeyNumber)) {
		if (InSkillSlot->IsVaild()) {
			// 从Skills池子里找 这个槽位匹配的GA
			if (UMMOARPGGameplayAbility* InMMOARPGGameplayAbility = GetGameplayAbilityForSkills(InSkillSlot->SkillName)) {
				// 再进一步检查 这个GA的条件标签是否匹配了 活跃标签组
				return InMMOARPGGameplayAbility->ConditionalActivationTags.HasAny(*(AbilitySystemComponent->GetCurrentActiveSkillTags()));
			}
		}
	}
	return false;
}

// 通过标签添加闪避诱发的霸体效果BUFF
void UFightComponent::ApplyDodgeEffect()
{
	if (FMMOARPGSkillSlot* InMMOARPGSkillSlot = FindSkillSlot((int32)EMMOARPGSkillType::DODGE_SKILL)) {
		// 对应键位查找到的闪避技能GA名称合法
		if (InMMOARPGSkillSlot->IsVaild()) {
			if (UMMOARPGGameplayAbility* InGA = GetGameplayAbilityForSkills(InMMOARPGSkillSlot->SkillName)) {// 此skills池子里提取技能
				for (auto& AnyMMOGEPak : InGA->EffectMap) {// 查询由本GA诱发的一组buff
					const static FName HitTag = TEXT("Player.Buff.Damage");
					if (!AnyMMOGEPak.Key.MatchesTag(FGameplayTag::RequestGameplayTag(HitTag))) {// 排除掉受击,受击的时候不能躲避
						/** 给自己本人的ASC应用BUFF效果 */
						for (TSubclassOf<UGameplayEffect>& TargetEffectTmp : AnyMMOGEPak.Value.TargetEffectClasses) {
							AbilitySystemComponent->ApplyGameplayEffectToSelf(
								Cast<UGameplayEffect>(TargetEffectTmp->GetDefaultObject()),
								MMOARPGCharacterBase->GetCharacterLevel(),
								AbilitySystemComponent->MakeEffectContext()
							);
						}
					}
				}
			}
		}
	}
}

// 按技能来源分型(Skill/Combo/Limb),激活指定名字GA
void UFightComponent::ExecuteGameplayAbility(EMMOARPGGameplayAbilityType InMMOGAType, const FName& InName)
{
	switch (InMMOGAType) {
		case EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK:
		{
			Attack_Combo(InName);
			break;
		}
		case EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK:
		{
			Skill(InName);
			break;
		}
		case EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_LIMBS:
		{
			Limb(InName);
			break;
		}
	}
}

// 让指定GE BUFF效果应用至自身ASC
void UFightComponent::ExecuteGameplayEffect(const TSubclassOf<UGameplayEffect>& InCommonGE)
{
	if (!InCommonGE) {
		return;
	}

	if (MMOARPGCharacterBase.IsValid()) {
		/* 用ASC构建1个GE-Context句柄, 并用指定GE填充它*/
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(MMOARPGCharacterBase.Get());
		FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InCommonGE, MMOARPGCharacterBase->GetCharacterLevel(), EffectContext);

		/* 应用上文构建好的GE至自身ASC*/
		if (EffectSpecHandle.IsValid()) {
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}
}
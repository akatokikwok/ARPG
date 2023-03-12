#include "MMOARPGGameplayAbility.h"
#include "MMOARPGAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask/AbilityTask_PMAWDamageEvent.h"
#include "AbilitySystemComponent.h"
#include "../MMOARPGPlayerController.h"
#include "../Character/MMOARPGCharacter.h"

UMMOARPGGameplayAbility::UMMOARPGGameplayAbility()
	: ConditionalSkillStartPos(0.1f)
	, ConditionalSkillDuration(0.3f)
{

}

void UMMOARPGGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UnregisterActiveSkillTag();// 卸除活跃标签组

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMMOARPGGameplayAbility::OnCompleted()
{
	K2_OnCompleted();
}

void UMMOARPGGameplayAbility::OnBlendOut()
{
	K2_OnBlendOut();
}

void UMMOARPGGameplayAbility::OnInterrupted()
{
	K2_OnInterrupted();
}

void UMMOARPGGameplayAbility::OnCancelled()
{
	K2_OnCancelled();
}

/** 拿取本GA里指定蒙太奇里的总段数. */
int32 UMMOARPGGameplayAbility::GetCompositeSectionsNumber()
{
	if (MontageToPlay) {
		return MontageToPlay->CompositeSections.Num();
	}
	return 0;
}

// 读取出buff在特定等级和在特定属性下的耗费值
float UMMOARPGGameplayAbility::CostValue(const FString& InCostName, float InLevel)
{
	if (UGameplayEffect* InCostBuff = GetCostGameplayEffect()) {// 找到会执行消耗的buff

		for (auto& Modifier : InCostBuff->Modifiers) {// 检索一系列modifier,这些modifier可能是血 蓝 耐力之类
			if (Modifier.Attribute.GetName() == InCostName) {// 找到入参指定的那个modifier名字,比如对耗蓝感兴趣
				float InValue = 0.f;
				if (Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, InValue)) {// 若成功读取到 这个属性在特定等级造成的值(比如一秒耗费5滴蓝)
					return FMath::Abs(InValue);
				}
			}
		}
	}

	return 0.0f;
}

/** 仿UAbilityTask_PlayMontageAndWait创建静态节点并绑定代理. */
UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::CreatePlayMontageAndWaitProxy(/*UGameplayAbility* OwningAbility, */FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/, float StartTimeSeconds /*= 0.f*/)
{
	// 构建1个AT节点; <UAbilityTask_PNAWDamageEvent>型;
	UAbilityTask_PNAWDamageEvent* InWait_Node =
		UAbilityTask_PNAWDamageEvent::CreatePNAWDamageEventProxy(this, TaskInstanceName, InMontageToPlay, AbilityTags, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale, StartTimeSeconds);

	if (InWait_Node != nullptr) {
		InWait_Node->OnCompleted.AddDynamic(this, &UMMOARPGGameplayAbility::OnCompleted);
		InWait_Node->OnBlendOut.AddDynamic(this, &UMMOARPGGameplayAbility::OnBlendOut);
		InWait_Node->OnInterrupted.AddDynamic(this, &UMMOARPGGameplayAbility::OnInterrupted);
		InWait_Node->OnCancelled.AddDynamic(this, &UMMOARPGGameplayAbility::OnCancelled);
		InWait_Node->DamageEventReceived.AddDynamic(this, &UMMOARPGGameplayAbility::OnDamageGameplayEvent);// 受击委托也要绑定回调.


		InWait_Node->Activate();// 激活ATNode
		//InWait_Node->ReadyForActivation();
		return InWait_Node;
	}

	return nullptr;
}

/** 播蒙太奇的哪个section */
UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::PlayMontageAnim(FName StartSection /*= NAME_None*/)
{
	return CreatePlayMontageAndWaitProxy(NAME_None, MontageToPlay, 1.f, StartSection);
}

// <UAbilityTask_PNAWDamageEvent>节点 受击委托绑定的回调.
// 回调, 专门负责解析传入的事件标签和事件数据
// 接收BoxHit那边SendGameplayEventToActor这个API传过来的伤害事件与标签组
// 虚方法,允许派生类的GA覆写
void UMMOARPGGameplayAbility::OnDamageGameplayEvent(FGameplayTag InGameplayTag, FGameplayEventData Payload)
{
	/** 一个事件标签对应 一组MMOBUFF, 用给定事件tag查询 */
	if (FMMOARPGGameplayEffects* InEffectArray = EffectMap.Find(InGameplayTag)) {
		
		/* 有一份Buff实例包待填充. */
		FMMOARPGGameplayEffectSpec MMOSpec_Pak;
		///  待注册的"BUFF-SPEC包"填充逻辑 与 施击和受击具体注册逻辑与编排顺序.
		/// 本质上就是为FMMOARPGGameplayEffectSpec类型的BUFF-SPEC包填充属性和数据, 然后再应用GE至Target
		{
			/** 1. new 1个buff作用对象的ActorArray, 视业务情况而填充; 使用GAS框架里推荐的 FGameplayAbilityTargetData_ActorArray */
			FGameplayAbilityTargetData_ActorArray* NewTargetData_ActorArray = new FGameplayAbilityTargetData_ActorArray();

			/** 2: 扫描所有MMOGE (每份MMOGE有自己的释放类型和本真GE).*/
			for (FMMOARPGGameplayEffect& AnyMMOGE : InEffectArray->MMOGEs) {

				/* 2.1 按作用类型填充ActorArray, 加工好的ActorArray再填进BUFF-SPEC包.*/
				if (AnyMMOGE.GameplayEffectUseType == EGameplayEffectUseType::RELEASE_OWN_COLLISION) {// 判断是给自己的被动buff
					NewTargetData_ActorArray->TargetActorArray.Add(const_cast<AActor*>(Payload.Instigator));
					MMOSpec_Pak.TargetHandleData.Add(NewTargetData_ActorArray);
				}
				else if (AnyMMOGE.GameplayEffectUseType == EGameplayEffectUseType::RELEASE_TARGET_COLLISION) {// 判断是给他人的被动buff
					NewTargetData_ActorArray->TargetActorArray.Add(const_cast<AActor*>(Payload.Target));
					MMOSpec_Pak.TargetHandleData.Add(NewTargetData_ActorArray);
				}
				else {
					break;
				}

				/* 2.2 用每份MMOGE 以及当前GAspec 构建各自的GAS-GE句柄. */
				FGameplayEffectSpecHandle MMOGEHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(AnyMMOGE.EffectClass, 1, MakeEffectContext(CurrentSpecHandle, CurrentActorInfo));

				/* 2.3 赠送GASepc的技能Tag给一份给 空GE句柄; 再用新建的GE句柄填充MMO-GE包. */
				if (MMOGEHandle.IsValid()) {
					// 先提取本GA的spec
					FGameplayAbilitySpec* AbilitySpec = GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromHandle(CurrentSpecHandle);
					// 把挂在GA实例上的Tag赠送一份给GE实例
					UGameplayAbility::ApplyAbilityTagsToGameplayEffectSpec(*MMOGEHandle.Data.Get(), AbilitySpec);
					// Buff-Spec包被注册另外一些数据
					if (AbilitySpec) {
						MMOGEHandle.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;// 用GA的标签量级填充GE的标签量级
						MMOSpec_Pak.TargetEffectSpecs.Add(MMOGEHandle);
					}
				}
			}
		}

		/** 3: 做完值的BUFF-SPEC包里提取出所有GE (伤害GE)应用给敌人. */
		for (FGameplayEffectSpecHandle& GEHandle : MMOSpec_Pak.TargetEffectSpecs) {
			TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles = K2_ApplyGameplayEffectSpecToTarget(GEHandle, MMOSpec_Pak.TargetHandleData);
		}
	}
}

// 在客户端更新CD
void UMMOARPGGameplayAbility::CallUpdateCooldownOnClient()
{
	if (UGameplayEffect* InCooldownBuff = GetCooldownGameplayEffect()) {/* 用本GA查询一下是否蓝图有配置CD的buff */
		float CDValue = 0.f;
		if (InCooldownBuff->DurationMagnitude.GetStaticMagnitudeIfPossible(this->GetAbilityLevel(), CDValue) && // 通过GA等级拿到冷却buff里的步幅值 (即CD值)
			CDValue != 0.f) {
			if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(this->GetActorInfo().OwnerActor)) {// 通过GetActorInfo拿玩家
				if (AMMOARPGPlayerController* InPlayerController = Cast<AMMOARPGPlayerController>(InCharacterBase->GetController())) {// 再拿到人的控制器
					// 让controller通知客户端更新CD
					InPlayerController->CallUpdateCooldownOnClient(*(this->AbilityTags).ToStringSimple(), CDValue);
				}
			}
		}
	}
}

void UMMOARPGGameplayAbility::RegisterActiveSkillTag()
{
	if (UMMOARPGAbilitySystemComponent* InMMOARPGCompent = Cast<UMMOARPGAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent)) {
		InMMOARPGCompent->SetCurrentActiveSkillTags(UGameplayAbility::AbilityTags);// 本技能的标签 给注册进活跃组里
	}
}

void UMMOARPGGameplayAbility::UnregisterActiveSkillTag()
{
	if (UMMOARPGAbilitySystemComponent* InMMOARPGCompent = Cast<UMMOARPGAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent)) {
		InMMOARPGCompent->ResetCurrentActiveSkillTags();
	}
}

// 提交条件分型的技能(对其蒙太奇做出播放时长的控制与处理)
void UMMOARPGGameplayAbility::CommitAbilityConditionalSkills(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (this->MontageToPlay != nullptr /*&& ConditionalActivationTags.IsValid()*/) {
		// 限制开始的位置必须小于蒙太奇总长度
		float Length = MontageToPlay->GetPlayLength();
		if (ConditionalSkillStartPos >= Length) {
			check(0);
			return;
		}

		if (Length != 0.f) {
			if (AMMOARPGCharacter* InMMOChar = Cast<AMMOARPGCharacter>(ActorInfo->AvatarActor)) {// ASC作用到的pawn
				// 播放条件技能(在客户端)
				InMMOChar->ConditionalSkillsOnClient(*AbilityTags.ToStringSimple(),// 本技能名字
					this->ConditionalSkillStartPos,// 条件型技能 蒙太奇的起始位置
					this->ConditionalSkillDuration,// 条件型技能 蒙太奇的起效果的持续时长
					Length// 蒙太奇总时长
				);
			}
		}
	}
}
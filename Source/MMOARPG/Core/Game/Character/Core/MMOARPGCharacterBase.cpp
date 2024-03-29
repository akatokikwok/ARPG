﻿// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGCharacterBase.h"
#include "../../MMOARPGGameState.h"
#include "Net/UnrealNetwork.h"
#include "../../Animation/Instance/Core/MMOARPGAnimInstanceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MMOARPG/MMOARPGGameType.h"
#include "../../../Component/FlyComponent.h"
#include "../../../Component/SwimmingComponent.h"
#include "../../../Component/ClimbingComponent.h"
#include "../../../Component/FightComponent.h"
#include "../../Abilities/MMOARPGAttributeSet.h"
#include "SimpleDrawTextFunctionLibrary.h"
#include "ThreadManage.h"
#include "../../../../UI/Game/Character/UI_CharacterHealthWidget.h"
#include "../../../Common/MMOARPGGameInstance.h"
#include "../../MMOARPGPlayerState.h"
#include "../../MMOARPGHUD.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMMOARPGCharacterBase::AMMOARPGCharacterBase()
	: ActionState(ECharacterActionState::NORMAL_STATE)
	, LastActionState(ECharacterActionState::NORMAL_STATE)
	, ID(INDEX_NONE)
	, UserID(INDEX_NONE)
	, LastHealth(0.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 构造一系列组件.
	FlyComponent = CreateDefaultSubobject<UFlyComponent>(TEXT("FlightComponent"));
	SwimmingComponent = CreateDefaultSubobject<USwimmingComponent>(TEXT("SwimmingComponent"));
	ClimbingComponent = CreateDefaultSubobject<UClimbingComponent>(TEXT("ClimbingComponent"));
	ClimbingComponent->SetIsReplicated(true);
	FightComponent = CreateDefaultSubobject<UFightComponent>(TEXT("FightComponent"));
	FightComponent->SetIsReplicated(true);
	AbilitySystemComponent = CreateDefaultSubobject<UMMOARPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);// 开启本ASC同步.

	AttributeSet = CreateDefaultSubobject<UMMOARPGAttributeSet>(TEXT("AttributeSet"));

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);
	Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 	HideWidget();// 默认隐藏血量UMG

	// 复位血条UI血量计时结束的那一刻 绑定一个代理效果:
	bResetWidget.Fun.BindLambda([&]() {
		HideWidget();
		});

	// 振刀信号默认关闭
	bVibratingKnife = false;
}

void AMMOARPGCharacterBase::HideWidget()
{
	if (UWidget* InWidget = GetWidget()) {
		InWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// 显示血条UMG(并同时设定了血量显隐计时器寿命)
void AMMOARPGCharacterBase::ShowWidget()
{
	if (UWidget* InWidget = GetWidget()) {
		bResetWidget = 15.f;// 计时器寿命设定15秒
		bResetWidget = true;// 计时器设定启用.
		InWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

float AMMOARPGCharacterBase::GetCharacterLevel()
{
	return AttributeSet->GetLevel();
}

float AMMOARPGCharacterBase::GetCharacterHealth()
{
	if (AttributeSet) {
		return AttributeSet->GetHealth();
	}

	return 0.f;
}

float AMMOARPGCharacterBase::GetCharacterMana()
{
	if (AttributeSet) {
		return AttributeSet->GetMana();
	}

	return 0.f;
}

float AMMOARPGCharacterBase::GetCharacterExp()
{
	if (AttributeSet) {
		return AttributeSet->GetEmpiricalValue();
	}

	return 0.f;
}

void AMMOARPGCharacterBase::UpdateLevel(float InLevel)
{
	if (FightComponent) {
		FightComponent->UpdateLevel(InLevel, UpgradeRewardEffect);
	}
}

UAbilitySystemComponent* AMMOARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AMMOARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// 客户端/服务端都应执行如下逻辑:
	if (GetWorld()) {
		if (AMMOARPGGameState* InGS = GetWorld()->GetGameState<AMMOARPGGameState>()) {// 再拿GS
// 把GS里的动画数据解算到Character身上.
			if (FCharacterAnimTable* InAnimRowData = InGS->GetCharacterAnimTable(this->GetID())) {
				this->AnimTable = InAnimRowData;
			}
		}
		if (!GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {// 服务器没必要执行IK.
			if (GetMesh()) {
				if (UMMOARPGAnimInstanceBase* InMMOARPGAnimInstanceBase = Cast<UMMOARPGAnimInstanceBase>(GetMesh()->GetAnimInstance())) {
					InMMOARPGAnimInstanceBase->InitAnimInstance(this);// 拿到动画实例并构建IK数据.
				}
			}
		}
		else {/* 位于服务器上*/

	  // 激活持续恢复buff,运行在协程中
			GThread::Get()->GetCoroutines().BindLambda(0.5f,
				[&]() ->void {
					ActivateRecoveryEffect();
				});
		}

		/* 在人内部 给ASC注册GAS属性集.*/
		TArray<UAttributeSet*> AttributeSets;
		AttributeSets.Add(AttributeSet);
		AbilitySystemComponent->SetSpawnedAttributes(AttributeSets);

		/* 关于血条umg里 姓名注入的逻辑.*/
		if (GetLocalRole() != ENetRole::ROLE_Authority) {
			if (UUI_CharacterHealthWidget* InWidget = Cast<UUI_CharacterHealthWidget>(this->GetWidget())) {
				// 非敌对势力下: 血条UMG姓名设定为 用户的姓名.
				if (GetCharacterType() < ECharacterType::CHARACTER_NPC_RESIDENT) {
					if (UMMOARPGGameInstance* InGameInstance = Cast<UMMOARPGGameInstance>(GetWorld()->GetGameInstance())) {
						InWidget->SetCharacterName(FText::FromString(InGameInstance->GetUserData().Name));
					}
				}
				// 敌对势力下: 设定为DT里找到的姓名.
				else {
					if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
						if (FCharacterStyleTable* InStyleTable = InGameState->GetCharacterStyleTable(GetID())) {
							//  拿到DTR_游戏人物后, 把姓名注入血条UMG
							InWidget->SetCharacterName(InStyleTable->CharacterName);
						}

						// 复位血量.
						if (FCharacterAttributeTable* InAttributeTable = InGameState->GetCharacterAttributeTable(GetID())) {
							LastHealth = InAttributeTable->Health;
						}
					}
				}
			}
		}
		//
		HideWidget();// 默认隐藏血量UMG
	}
}

// Called every frame
void AMMOARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ENetRole::ROLE_Authority) {

		if (IsDie()) {
			HideWidget();
		}
		else {
			// 再细分一下非主机客户端(即只剩下模拟玩家)的逻辑.
			if (GetLocalRole() != ENetRole::ROLE_AutonomousProxy) {
				// 如若转成悬浮人物血条UMG.
				if (UUI_CharacterHealthWidget* InWidget = Cast<UUI_CharacterHealthWidget>(this->GetWidget())) {
					// 属性是来自服务器同步过来的属性集
					if (AttributeSet != nullptr) {
						if (LastHealth != AttributeSet->GetHealth()) {
							// 显示血条UMG(并同时设定了血量显隐计时器寿命)
							ShowWidget();// 当本帧的血量产生变动才会显示UMG; 
						}

						InWidget->SetLv(AttributeSet->GetLevel());
						InWidget->SetHealth(AttributeSet->GetHealth() / AttributeSet->GetMaxHealth());

						LastHealth = AttributeSet->GetHealth();// 记录最新本帧的血量.
					}
				}
				// Tick血条UMG计时器.
				bResetWidget.Tick(DeltaTime);
			}
		}
	}
}

// Called to bind functionality to input
void AMMOARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 给特定的信号值,然后实现对应的notify逻辑; 覆写ISimpleCombatInterface::AnimSignal.
void AMMOARPGCharacterBase::AnimSignal(int32 InSignal)
{
	K2_AnimSignal(InSignal);

	// 手动配置一下7和8,播任一一种死亡seq anim.
	if (InSignal == 7) {
		DieIndex = 0;
	}
	else if (InSignal == 8) {
		DieIndex = 1;
	}
	else if (InSignal == 9) {// 9激活本角色人物被挑飞状态动画
		GetFightComponent()->bPickFly = true;
	}
	else if (InSignal == 10) {// 10停用本角色人物被挑飞状态动画(即落地)
		GetFightComponent()->bPickFly = false;
	}
	else if (InSignal == 11) {// 11 是起身
		GetUp();
	}
	else if (InSignal == 15) {//开始振刀
		bVibratingKnife = true;
	}
	else if (InSignal == 16) {//结束振刀
		bVibratingKnife = false;
	}
	else if (InSignal == 20)// 持续施法以耗蓝为主
	{
		if (GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {// 仅承载在服务端有效
			if (FContinuousReleaseSpell* InReleaseSpell = GetContinuousReleaseSpell()) {
				if (UGameplayEffect* InGE = Cast<UGameplayEffect>(InReleaseSpell->BuffPtrObj)) {/* 这种情况是发动了持续施法技能且注册了消耗*/
					float OutMagnitude = 0.f;
					for (auto& Modifier : InGE->Modifiers) {
						// 仅去找修改蓝的部分,取出这个数值
						if (Modifier.Attribute.AttributeName == TEXT("Mana")) {
							Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(GetCharacterLevel(), OutMagnitude);
							break;
						}
					}

					OutMagnitude = FMath::Abs(OutMagnitude);
					// 人的蓝量过低,无法再发动了, 直接把黑盒计数调整为2,中止再播为1时候的循环动画
					if (GetCharacterMana() < OutMagnitude) {
						ContinuousReleaseSpellEndOnMulticast();
					}
				}
				else {/* 这种是未发动持续施法,没有注册持续施法消耗*/
					ContinuousReleaseSpellEndOnMulticast();// 
				}
			}
		}
	}
}

// 用1行DTR属性 注册更新AttributeSet指针数据
void AMMOARPGCharacterBase::UpdateAttribute(const FCharacterAttributeTable* InDTRowAttribute)
{
	if (AttributeSet) {
		AttributeSet->RegistrationProperties(InDTRowAttribute);
	}
}

// 用1个GAS属性集 注册更新AttributeSet指针数据
void AMMOARPGCharacterBase::UpdateAttribute(const FMMOARPGCharacterAttribute* InGASAttribute)
{
	if (AttributeSet) {
		AttributeSet->RegistrationProperties(*InGASAttribute);
	}
}

void AMMOARPGCharacterBase::ResetActionState(ECharacterActionState InNewActionState)
{
	//客户端
	if (ActionState == InNewActionState) {
		ActionState = ECharacterActionState::NORMAL_STATE;
	}
	else {
		ActionState = InNewActionState;
	}
}

// 检查人物是否死亡.
bool AMMOARPGCharacterBase::IsDie()
{
	if (AttributeSet) {
		return AttributeSet->GetHealth() <= 0.f;// 查询属性集内部 health字段情况.
	}
	return true;// 否则默认其死亡
}

// 同步变量需要重写的方法.
void AMMOARPGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 手动条件注册,条件变量 使用此宏网络同步 bFight字段.
	// 此处设定规则是COND_SimulatedOnly,意为当主机本机客户端发生了某些行为后, 这些行为只同步给模拟玩家.
	DOREPLIFETIME_CONDITION(AMMOARPGCharacterBase, ActionState, COND_SimulatedOnly);
}

void AMMOARPGCharacterBase::SwitchActionStateOnServer_Implementation(ECharacterActionState InActionState)
{
	ActionState = InActionState;

	LastActionState = ActionState;// 刷新最后姿态.
}

void AMMOARPGCharacterBase::OnRep_ActionStateChanged()
{
	// 虚方法,在其他地方实现.
}

// 重写基类; 落地(可能是飞行落地,或者是攀岩坠落落地)
void AMMOARPGCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	// 在坠地着陆的时候 清掉攀岩状态的一切播的动画.
// 	if (LastActionState == ECharacterActionState::CLIMB_STATE) 
	{
		// 		StopAnimMontage();
	}

}

// 放平砍技能.
void AMMOARPGCharacterBase::NormalAttack(const FName& InKey)
{
	GetFightComponent()->Attack_Combo(InKey);
}

// 覆盖ISimpleComboInterface::ComboAttack; 本质上执行战斗组件放出平砍GA.
void AMMOARPGCharacterBase::ComboAttack(const FName& InKey)
{
	this->NormalAttack(InKey);
}

// 拿取当前人物身份类型(用以敌我识别)
ECharacterType AMMOARPGCharacterBase::GetCharacterType()
{
	return MMOARPGGameMethod::GetCharacterType(GetID());
}

// 移除死亡后遗体
void AMMOARPGCharacterBase::RemoveDeadBody(float InTime /*= 4.f*/)
{
	GThread::Get()->GetCoroutines().BindLambda(InTime, [&]() {
		Destroy();
		});
}

// 检查本角色是否被挑飞
bool AMMOARPGCharacterBase::IsPickFly()
{
	if (GetFightComponent()) {
		return GetFightComponent()->bPickFly;
	}
	return false;
}

// 拿取Widget组件里真正的UMG(仅在客户端).
UWidget* AMMOARPGCharacterBase::GetWidget()
{
	if (Widget) {
		return Cast<UWidget>(Widget->GetUserWidgetObject());// 从widget组件里使用此接口获取真正的UMG
	}
	return nullptr;
}

struct FSimpleComboCheck* AMMOARPGCharacterBase::GetSimpleComboInfo(const FName& InGAKey)
{
	return GetFightComponent()->GetSimpleComboInfo(InGAKey);
}

struct FContinuousReleaseSpell* AMMOARPGCharacterBase::GetContinuousReleaseSpell()
{
	if (GetFightComponent()) {
		return GetFightComponent()->GetContinuousReleaseSpell();
	}
	return nullptr;
}

// 广播 刷新最新的人物GAS属性集.
void AMMOARPGCharacterBase::UpdateCharacterAttribute_Implementation(const FMMOARPGCharacterAttribute& CharacterAttribute)
{
	UpdateAttribute(&CharacterAttribute);
}

// 处理人的血量; 虚方法
void AMMOARPGCharacterBase::HandleHealth(AMMOARPGCharacterBase* InstigatorPawn, AActor* DamageCauser, const struct FGameplayTagContainer& InTags, float InNewValue, bool bPlayHit)
{
	if (FightComponent != nullptr) {
		FightComponent->HandleHealth(InstigatorPawn, DamageCauser, InTags, InNewValue, bPlayHit);
	}
}

// 处理人的蓝量; 虚方法
void AMMOARPGCharacterBase::HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue)
{
	if (FightComponent != nullptr) {
		FightComponent->HandleMana(InTags, InNewValue);
	}
}

// 处理人的伤害值; 虚方法
void AMMOARPGCharacterBase::HandleDamage(float DamageAmount,/* 伤害值 */ const struct FGameplayTagContainer& DamageTags,/* 标签 */ AMMOARPGCharacterBase* InstigatorPawn,/* 施法者 */ AActor* DamageCauser/* 源ASC内的源actor */)
{
	// 生成飘动在人头部上侧的字体.它会自动销毁.
	FVector InNewLocation = GetActorLocation();
	InNewLocation.Z += 140.f;

	// 执行2遍是因为为了让挨打的和开打的2个客户端都可以看见伤害值.
	InstigatorPawn->SpawnDrawTextInClient(DamageAmount, InNewLocation, 0.8f);
	SpawnDrawTextInClient(DamageAmount, InNewLocation, 0.8f);
}

// 
void AMMOARPGCharacterBase::HandleExp(const struct FGameplayTagContainer& InTags, float InNewValue)
{

}

// 写入战斗组件里的受击ID
void AMMOARPGCharacterBase::SetHitID(int32 InNewID)
{
	FightComponent->HitID = InNewID;
}

// 读取战斗组件里的受击ID
const int32 AMMOARPGCharacterBase::GetHitID() const
{
	return FightComponent->HitID;
}

// 执行受击
void AMMOARPGCharacterBase::PlayHit()
{
	GetFightComponent()->Hit();
}

// 执行死亡
void AMMOARPGCharacterBase::PlayDie()
{
	GetFightComponent()->Die();

	// 解除持续恢复buff
	DeactivationRecoveryEffect();
}

// 使用战斗组件里的 注册各部分技能(按形式来源)
void AMMOARPGCharacterBase::RegisterGameplayAbility(const TArray<FName>& InGANames/*一组技能名*/, EMMOARPGGameplayAbilityType InGASrcEnum/*技能形式来源*/)
{
	if (FightComponent != nullptr) {
		FightComponent->RegisterGameplayAbility(InGANames, InGASrcEnum);
	}
}

// RPC至客户端, 让客户端播放伤害字体.
void AMMOARPGCharacterBase::SpawnDrawTextInClient_Implementation(float InDamageAmount, const FVector& InLocation, float InRate)
{
	USimpleDrawTextFunctionLibrary::SpawnDrawText(
		GetWorld(),
		InLocation,
		FString::Printf(TEXT("- %.2lf"), InDamageAmount), FColor::Red, InRate);
}

// "单机非广播版" 用一组GA去注册1个连招黑盒
void AMMOARPGCharacterBase::RegisterComboAttack(const TArray<FName>& InGANames)
{
	if (FightComponent) {
		FightComponent->RegisterComboAttack(InGANames);
	}
}

void AMMOARPGCharacterBase::MontagePlayOnServer_Implementation(UAnimMontage* InNewAnimMontage, float InPlayRate, float InTimeToStartMontageAt /*= 0.f*/, bool bStopAllMontages /*= true*/, FName InStartSectionName /*= NAME_None*/, EMMOARPGSkillReleaseType ReleaseType)
{
	if (InNewAnimMontage) {
		MontagePlayOnMulticast(InNewAnimMontage, InPlayRate, InTimeToStartMontageAt, bStopAllMontages, InStartSectionName);
	}
}

void AMMOARPGCharacterBase::StopAnimMontageOnMulticast_Implementation()
{
	ACharacter::StopAnimMontage();
}

// 让动画实例播1个蒙太奇的指定section
void AMMOARPGCharacterBase::MontagePlayOnMulticast_Implementation(UAnimMontage* InNewAnimMontage, float InPlayRate, float InTimeToStartMontageAt /*= 0.f*/, bool bStopAllMontages /*= true*/, FName InStartSectionName /*= NAME_None*/, EMMOARPGSkillReleaseType ReleaseType)
{
	if (GetMesh() && InNewAnimMontage) {
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			/** 在动画实例内 Play a Montage. Returns Length of Montage in seconds. Returns 0.f if failed to play. */
			if (AnimInstance->Montage_Play(InNewAnimMontage, InPlayRate, EMontagePlayReturnType::MontageLength, InTimeToStartMontageAt, bStopAllMontages) > 0.f) {
				// Start at a given Section.
				if (InStartSectionName != NAME_None) {
					AnimInstance->Montage_JumpToSection(InStartSectionName, InNewAnimMontage);
				}
			}

			// 针对施法类型是持续施法型的,则保存一份section内的动画
			if (ReleaseType == EMMOARPGSkillReleaseType::CONTINUOUS) {
				if (FContinuousReleaseSpell* InReleaseSpell = GetContinuousReleaseSpell()) {
					// 保存一份
					InReleaseSpell->AnimMontage = InNewAnimMontage;
				}
			}
		}
	}
}

void AMMOARPGCharacterBase::GetUpOnMulticast_Implementation()
{
	GetUp();
}

void AMMOARPGCharacterBase::EnableGravityMulticast_Implementation(float bDelayTime)
{
	if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(this->GetMovementComponent())) {
		InCMC->GravityScale = 0.f;
		InCMC->StopMovementImmediately();

		// 设置为1s后回复原样~
		GThread::Get()->GetCoroutines().BindLambda(bDelayTime, [InCMC]() {
			InCMC->GravityScale = 1.f;
			});
	}
}

void AMMOARPGCharacterBase::PlayResidualShadowMulticast_Implementation()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority) {
		// 需要在服务端应用那些由闪避技能诱发的buff(如霸体效果)
		ApplyDodgeEffect();
	}
	else {
		// 仅在客户端生成闪避残影
		SpawnResidualShadowActor();
	}
}

// 播放慢动作/子弹时间效果(仅客户端)
void AMMOARPGCharacterBase::PlaySlowMotionOnClient_Implementation(float InDuration, float InSpeed)
{
	// 先设定一下全局时间膨胀倍率
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), InSpeed);

	// 持续一段时长后, 时间膨胀复位.
	GThread::Get()->GetCoroutines().BindLambda(InDuration, [&]() {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		});
}

// 持续施法计数设定为2号,勒令施法动画End (广播至任意客户端)
void AMMOARPGCharacterBase::ContinuousReleaseSpellEndOnMulticast_Implementation()
{
	// 认为黑盒设置为2号就是执行持续施法
	if (FContinuousReleaseSpell* ReleaseSpell = GetContinuousReleaseSpell()) {
		ReleaseSpell->ContinuousReleaseSpellIndex = 2;
	}
}

// 授予击杀本人物的奖励Buff
void AMMOARPGCharacterBase::RewardEffect(float InNewLevel, TSubclassOf<UGameplayEffect> InNewRewardBuff, TFunction<void()> InFun)
{
	if (FightComponent) {
		FightComponent->RewardEffect(InNewLevel, InNewRewardBuff, InFun);
	}
}

// 判断是否满足升人物等级条件.
bool AMMOARPGCharacterBase::IsUpdateLevel()
{
	if (AttributeSet) {
		return AttributeSet->GetEmpiricalValue() >= AttributeSet->GetMaxEmpiricalValue();
	}
	return false;
}

void AMMOARPGCharacterBase::GetSkillTagsName(TArray<FName>& OutNames)
{
	if (FightComponent) {
		FightComponent->GetSkillTagsName(OutNames);
	}
}

void AMMOARPGCharacterBase::GetComboAttackTagsName(TArray<FName>& OutNames)
{
	if (FightComponent) {
		FightComponent->GetComboAttackTagsName(OutNames);
	}
}

void AMMOARPGCharacterBase::GetLimbsTagsName(TArray<FName>& OutNames)
{
	if (FightComponent) {
		FightComponent->GetLimbsTagsName(OutNames);
	}
}

// 人物执行复活
void AMMOARPGCharacterBase::Resurrection()
{
	if (IsNetMode(ENetMode::NM_DedicatedServer)) {
		if (AttributeSet) {
			AttributeSet->SetHealth(AttributeSet->GetMaxHealth());
			AttributeSet->SetMana(AttributeSet->GetMaxMana());

			// 再次激活持续恢复buff
			ActivateRecoveryEffect();

			// 顺带播放 起身跳起动画
			GetUpOnMulticast();
		}
	}
}

void AMMOARPGCharacterBase::ActivateRecoveryEffect()
{
	if (FightComponent) {
		for (auto& Tmp : RecoveryEffect) {
			FightComponent->ActivateRecoveryEffect(Tmp);
		}
	}
}

void AMMOARPGCharacterBase::DeactivationRecoveryEffect()
{
	if (FightComponent) {
		for (auto& Tmp : RecoveryEffect) {
			FightComponent->DeactivationRecoveryEffect(Tmp);
		}
	}
}

// 播放 人物被击倒或挑飞后起身
void AMMOARPGCharacterBase::GetUp()
{
	if (FCharacterAnimTable* InAnimTable_row = AMMOARPGCharacterBase::GetAnimTable()) {
		if (InAnimTable_row->GetUpMontage != nullptr) {
			PlayAnimMontage(InAnimTable_row->GetUpMontage, // 蒙太奇资产: SwitchFight
				1.f
			);
		}
	}
}

bool AMMOARPGCharacterBase::IsAir()
{
	return GetMovementComponent()->IsFalling();
}

float AMMOARPGCharacterBase::GetCapsuleHalfHeight() const
{
	return GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void AMMOARPGCharacterBase::SetDaytonFrame(bool bDaytonFrame)
{
	if (APlayerController* InPlayerController = Cast<APlayerController>(GetController())) {
		InPlayerController->SetPause(bDaytonFrame);
	}
}

// 设置顿帧
void AMMOARPGCharacterBase::DaytonFrame(float InDuration)
{
	SetDaytonFrame(true);

	GThread::Get()->GetCoroutines().BindLambda(InDuration, [&]() {
		SetDaytonFrame(false);
		});
}

// 人物是否处于某种活跃标签的状态(即判断人现在放了哪个技能,身上有什么标签作为状态识别)
bool AMMOARPGCharacterBase::IsExitActiveTag(const FName& InGASTag)
{
	if (const FGameplayTagContainer* ActiveTags = AbilitySystemComponent->GetCurrentActiveSkillTags()) {
		// hasAny表示所有都选中,而非精确匹配至某一个
		return ActiveTags->HasAny(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(InGASTag)));
	}
	return false;
}

// 给pawn生成闪避残影Actor
bool AMMOARPGCharacterBase::SpawnResidualShadowActor()
{
	// 使用BP Library生成1个闪避残影
	if (USimpleCombatBPLibrary::SpawnResidualShadow(
		GetWorld(), ResidualShadowActorClass, GetMesh(),
		-GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		GetActorLocation(),
		GetActorRotation(), 1.5f) != nullptr) {
		return true;
	}
	return false;
}

// 如果人恰好在释放闪避技能, 则拿取闪避的技能状态标签
FName AMMOARPGCharacterBase::DodgeTags()
{
	// 给配装了闪避GA的 那个UI上对应的键位号, 去技能槽池子里找匹配的闪避GA
	if (FMMOARPGSkillSlot* InSkillSlot = FightComponent->FindSkillSlot((int32)EMMOARPGSkillType::DODGE_SKILL)) {
		return InSkillSlot->SkillName;
	}
	return NAME_None;
}

// 应用由闪避技能诱发出来的buff(比如闪避诱发了自身的霸体效果)
void AMMOARPGCharacterBase::ApplyDodgeEffect()
{
	if (FightComponent) {
		FightComponent->ApplyDodgeEffect();
	}
}

// 检查已激活的活跃Buff里是否匹配给定标签
bool AMMOARPGCharacterBase::IsActiveGameplayEffectTags(const FName& InTag)
{
	return AbilitySystemComponent->IsActiveGameplayEffectTags(FGameplayTag::RequestGameplayTag(InTag));
}

// 按技能来源分型(Skill/Combo/Limb),激活指定名字GA
void AMMOARPGCharacterBase::ExecuteGameplayAbility(EMMOARPGGameplayAbilityType InMMOGameplayAbilityType, const FName& InName)
{
	if (FightComponent) {
		FightComponent->ExecuteGameplayAbility(InMMOGameplayAbilityType, InName);
	}
}

// 让指定GE BUFF效果应用至自身ASC
void AMMOARPGCharacterBase::ExecuteGameplayEffect(const TSubclassOf<UGameplayEffect>& InGameplayEffect)
{
	if (FightComponent) {
		FightComponent->ExecuteGameplayEffect(InGameplayEffect);
	}
}

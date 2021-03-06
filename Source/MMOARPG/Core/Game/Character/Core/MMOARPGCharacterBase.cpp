// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGCharacterBase.h"
#include "../../MMOARPGGameState.h"
#include "../../Animation/Instance/Core/MMOARPGAnimInstanceBase.h"
#include "Net/UnrealNetwork.h"
#include "SimpleDrawTextFunctionLibrary.h"
#include "ThreadManage.h"


// Sets default values
AMMOARPGCharacterBase::AMMOARPGCharacterBase()
	: ActionState(ECharacterActionState::NORMAL_STATE)
	, LastActionState(ECharacterActionState::NORMAL_STATE)
	, ID(INDEX_NONE)
	, UserID(INDEX_NONE)
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
		if (!GetWorld()->IsServer()) {// 服务器没必要执行IK.
			if (GetMesh()) {
				if (UMMOARPGAnimInstanceBase* InMMOARPGAnimInstanceBase = Cast<UMMOARPGAnimInstanceBase>(GetMesh()->GetAnimInstance())) {
					InMMOARPGAnimInstanceBase->InitAnimInstance(this);// 拿到动画实例并构建IK数据.
				}
			}
		}

		/* 在人内部 给ASC注册GAS属性集.*/
		TArray<UAttributeSet*> AttributeSets;
		AttributeSets.Add(AttributeSet);
		AbilitySystemComponent->SetSpawnedAttributes(AttributeSets);
	}
}

// Called every frame
void AMMOARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	GetFightComponent()->Attack_TriggerGA(InKey);
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

struct FSimpleComboCheck* AMMOARPGCharacterBase::GetSimpleComboInfo()
{
	return GetFightComponent()->GetSimpleComboInfo();
}

// 广播 刷新最新的人物GAS属性集.
void AMMOARPGCharacterBase::UpdateCharacterAttribute_Implementation(const FMMOARPGCharacterAttribute& CharacterAttribute)
{
	UpdateAttribute(&CharacterAttribute);
}

// 处理人的血量; 虚方法
void AMMOARPGCharacterBase::HandleHealth(const struct FGameplayTagContainer& InTags, float InNewValue)
{

}

// 处理人的蓝量; 虚方法
void AMMOARPGCharacterBase::HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue)
{

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

// 广播 "用一组GA注册连招黑盒"
void AMMOARPGCharacterBase::RegisterComboAttackMulticast(const TArray<FName>& InGANames)
{
	if (FightComponent) {
		FightComponent->RegisterComboAttackMulticast(InGANames);
	}
}

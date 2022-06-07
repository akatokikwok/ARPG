// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGCharacterBase.h"
#include "../../MMOARPGGameState.h"
#include "../../Animation/Instance/Core/MMOARPGAnimInstanceBase.h"
#include "Net/UnrealNetwork.h"

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

void AMMOARPGCharacterBase::AnimSignal(int32 InSignal)
{
	K2_AnimSignal(InSignal);
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

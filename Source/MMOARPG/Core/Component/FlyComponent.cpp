// Fill out your copyright notice in the Description page of Project Settings.
#include "FlyComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UFlyComponent::UFlyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UFlyComponent::BeginPlay()
{
	Super::BeginPlay();
	if (MMOARPGCharacterBase.IsValid()) {

		if (CharacterMovementComponent.IsValid()) {
			CharacterMovementComponent->MaxAcceleration = 2500.f;// 最大加速度, 用于辅助调试飞行手感
			CharacterMovementComponent->BrakingDecelerationFlying = 1400.f;// 飞行减速, 用于辅助调试飞行手感
		}

		// 		MMOARPGCharacterBase->LandedDelegate.AddDynamic(this, &UFlyComponent::Landed);// LandedDelegate是蹦起来之后落地那一刻的代理

		if (CapsuleComponent.IsValid()) {
			CapsuleComponent->OnComponentHit.AddDynamic(this, &UFlyComponent::Landed);// 人的胶囊接触地面一刻给代理绑着落回调.
		}

		/// 疾飞计时结束后 绑定lambda: 刷新翻滚种类为none.
		bFast.Fun.BindLambda([&]() ->void {
			DodgeFly = EDodgeFly::DODGE_NONE;
			});
		/// 着陆计时结束后 绑定lambda: 
		bLand.Fun.BindLambda([&]() ->void {
			if (MMOARPGCharacterBase.IsValid()) {
				// 这一步保护一下,若落地仍是飞行姿态则强制切换为normal状态.
				MMOARPGCharacterBase->ResetActionState(ECharacterActionState::FLIGHT_STATE);
				// 还原一套慢飞姿态或站立姿态下的组件设置.
				ResetFly();
			}});
	}
}

// Called every frame
void UFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CharacterMovementComponent.IsValid() && MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() && CameraComponent.IsValid()) {

		if (MMOARPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE) {/* 仅在飞行姿态下. */
			
			/** 在服务器和主机玩家上都需要做的逻辑. (次一级衰弱权限的模拟玩家不需要进行,因为服务器执行的时候已经广播同步到客户端了). */
			if (MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority 
				|| MMOARPGCharacterBase->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
			{
				if (!bLand) {

					LockView(DeltaTime, *bFast);// 锁定视角.

					if (1) {/* 第一种算法*/

						float PreFrameNum = 1.f / DeltaTime;// 单秒帧数.
						FRotator NewDeltaTimeRot = MMOARPGCharacterBase->GetActorRotation() - LastRotator;// 单帧转动弧度.
						NewDeltaTimeRot *= PreFrameNum;// 一秒内转过的弧度.
						//Print(DeltaTime, NewDeltaTimeRot.ToString());
						RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1.f, 1.f), NewDeltaTimeRot.Yaw);
						RotationRate.Y = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1.f, 1.f), NewDeltaTimeRot.Pitch);
						LastRotator = MMOARPGCharacterBase->GetActorRotation();

					}
					else {/*第二种思路,备用*/

						/* 设置角速度(yaw上正负360度)并映射到混合空间里的人物头转向的的(-1,1)*/
						FVector  PhysicsAngularVelocityInDegrees = CapsuleComponent->GetPhysicsAngularVelocityInDegrees();// 通过胶囊体拿角速度.
						Print(DeltaTime, PhysicsAngularVelocityInDegrees.ToString());
						RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1, 1),
							PhysicsAngularVelocityInDegrees.Z// 绕Z就是Yaw
						);
						RotationRate.Y = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, 360.f), FVector2D(-1, 1),
							PhysicsAngularVelocityInDegrees.X// 绕x就是Pitch.
						);
					}
				}
			}
		}

		
		// 着陆Tick;
		bLand.Tick(DeltaTime);
	}
}

void UFlyComponent::ResetFly()
{
	if (CharacterMovementComponent.IsValid() && MMOARPGCharacterBase.IsValid()) {
		/**
		 * 需要在standAlone模式下去测试
		 * 因为SetMovementMode和网络有关系,standalone是服务器与客户端连为一体.
		 */

		if (MMOARPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE) {/*飞行姿态下*/
			CharacterMovementComponent->bOrientRotationToMovement = false;// 禁用随移动组件旋转.
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);// 切换为UE提供的飞行MODE,(即此MODE下无法跳等其他操作)
		}
		else {/*非飞行姿态.*/
			
			Reset();// 还原一套仅用于站立姿态下的组件设置.
		}
		bFast = false;// 慢速飞行下禁用加速飞行.
	}
}

void UFlyComponent::FlyForwardAxis(float InAxisValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CapsuleComponent.IsValid() &&
		CameraComponent.IsValid()) {

		if (bFast) {
			const FVector Direction = CameraComponent->GetForwardVector();

			if (InAxisValue > 0.0f) {// 在急速飞行下, 确实按下键盘键位w.
				MMOARPGCharacterBase->AddMovementInput(Direction, 1.0f);// 按相机指向的方向进行输入移动.但不接收外部value,只用1.0f保持永远前向而不是其他方向(例如后退)
			}
			else {// 没按下任意键盘键位或者按了后退键
				MMOARPGCharacterBase->AddMovementInput(Direction, 0.0f);
			}
		}
		else {
			const FVector Direction = CameraComponent->GetForwardVector();
			MMOARPGCharacterBase->AddMovementInput(Direction, InAxisValue);// 按相机指向的方向进行输入移动.
		}
	}
}

void UFlyComponent::ResetFastFly()
{
	if (CharacterMovementComponent.IsValid()) {
		if (bFast) {
			bFast = false;
			CharacterMovementComponent->MaxFlySpeed = 600.f;
		}
		else {
			bFast = true;
			CharacterMovementComponent->MaxFlySpeed = 2000.f;
		}
	}
}

void UFlyComponent::ResetDodgeFly(EDodgeFly InFlyState)
{
	if (bFast) {// 仅在加速飞行下生效
		DodgeFly = InFlyState;

		// 重刷新 延时器(关联翻滚的)延时时长.
		bFast = 1.6f;
	}
}

void UFlyComponent::Landed(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (MMOARPGCharacterBase.IsValid()) {
		if (MMOARPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE
			&& bFast) { /* 满足是疾飞*/

			/* 计算出人物前向与 撞击点法线的夹角角度(比如人落地时候相互垂直,是90度.)*/
 			float CosValue = FVector::DotProduct(CapsuleComponent->GetForwardVector(), Hit.ImpactNormal);
 			float CosAngle = (180.f) / PI * FMath::Acos(CosValue);

			/* 认为大于125度比较符合人倾斜着想要着陆. */
 			if (CosAngle >= 125.f) {
 				if (Hit.ImpactNormal.Z > 0.5f) {// 排除撞到山体或者竖墙的意外情况.

					Reset();// 还原一套仅用于站立姿态下的组件设置.
					bLand = true;// 着陆刷为真
					bLand = 1.6f;// 落地之后总计时再刷回去.
 				}
 			}

		}
	}
}

void UFlyComponent::Reset()
{
	CharacterMovementComponent->bOrientRotationToMovement = true;// 开启随移动组件旋转.
	CharacterMovementComponent->MaxFlySpeed = 600.f;
	CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);// 切换为UE提供的 Walking Mode.

	// 修正落地后人物pitch轴向是倾斜的.
	FRotator NewRot = MMOARPGCharacterBase->GetActorRotation();
	NewRot.Pitch = 0.0f;
	MMOARPGCharacterBase->SetActorRotation(NewRot);
}

void UFlyComponent::Print(float InTime, const FString& InString)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, InTime, FColor::Yellow, FString::Printf(TEXT("%s"), *InString));
	}
}


#include "Hit/Core/ComboSkillHitCollision.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "Components/SplineComponent.h"

//
AHitCollision::AHitCollision(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HitCollisionRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionRootComponent"));
	RootComponent = HitCollisionRootComponent;

	InitialLifeSpan = 4.0f;// 初始化本actor生命时长.
}

//
void AHitCollision::BeginPlay()
{
	Super::BeginPlay();

	if (UPrimitiveComponent* InHitComponent = GetHitDamage()) {
		
		Collision(false);// 默认先禁用碰撞.
		InHitComponent->SetHiddenInGame(false);
		InHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AHitCollision::HandleDamage);// 与场景物体相撞接触的时候绑定回调.
	}
}

//
void AHitCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 回调虚方法: 处理碰撞接触时候的逻辑;  可由子类覆写.
void AHitCollision::HandleDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* InCharacter = Cast<ACharacter>(OtherActor)) {
		AttackedTarget.AddUnique(InCharacter);
	}
}

// 虚方法,让派生类覆写; 拿取对应的形状comp;
UPrimitiveComponent* AHitCollision::GetHitDamage()
{
	return nullptr;
}

// 设置 具备形状comp 的相对位置.
void AHitCollision::SetHitDamageRelativePosition(const FVector& InNewPostion)
{
	if (UPrimitiveComponent* InHitComponent = GetHitDamage()) {
		InHitComponent->SetRelativeLocation(InNewPostion);
	}
}

// 操作启用或禁用hitbox碰撞.
void AHitCollision::Collision(bool bTurnOnCollision)
{
	if (UPrimitiveComponent* InHitComponent = GetHitDamage()) {
		if (bTurnOnCollision) {
			InHitComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else {
			InHitComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

bool AHitCollision::IsExist(ACharacter* InNewTarget) const
{
	for (auto& Tmp : AttackedTarget) {
		if (Tmp.IsValid()) {
			if (Tmp == InNewTarget) {
				return true;
			}
		}
	}
	return false;
}

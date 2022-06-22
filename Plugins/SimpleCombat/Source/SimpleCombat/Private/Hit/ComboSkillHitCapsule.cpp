#include "SimpleCombat/Public/Hit/ComboSkillHitCapsule.h"
#include "Components/CapsuleComponent.h"

//
AHitCapsuleCollision::AHitCapsuleCollision(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HitDamage = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitDamage"));
	HitDamage->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHitCapsuleCollision::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHitCapsuleCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 覆写基类的. 拿取胶囊型comp.
UPrimitiveComponent* AHitCapsuleCollision::GetHitDamage()
{
	return HitDamage;
}

// 影响外观; 设定胶囊的半径和半高
void AHitCapsuleCollision::SetCapsuleHalfHeight(float InNewCapsuleHalfHeight)
{
	HitDamage->SetCapsuleHalfHeight(InNewCapsuleHalfHeight);
}

// 影响外观; 设定胶囊的半径和半高
void AHitCapsuleCollision::SetCapsuleRadius(float InNewCapsuleRadius)
{
	HitDamage->SetCapsuleRadius(InNewCapsuleRadius);
}

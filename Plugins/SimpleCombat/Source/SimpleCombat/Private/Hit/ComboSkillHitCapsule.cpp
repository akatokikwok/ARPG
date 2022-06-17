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

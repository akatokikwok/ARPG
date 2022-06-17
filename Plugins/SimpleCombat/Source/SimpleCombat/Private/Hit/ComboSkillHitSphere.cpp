#include "SimpleCombat/Public/Hit/ComboSkillHitSphere.h"
#include "Components/SphereComponent.h"

AHitSphereCollision::AHitSphereCollision(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HitDamage = CreateDefaultSubobject<USphereComponent>(TEXT("HitDamage"));
	HitDamage->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHitSphereCollision::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHitSphereCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 覆写基类的.拿取球形Comp.
UPrimitiveComponent* AHitSphereCollision::GetHitDamage()
{
	return HitDamage;
}

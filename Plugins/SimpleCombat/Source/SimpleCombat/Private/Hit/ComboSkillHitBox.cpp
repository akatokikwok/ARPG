#include "SimpleCombat/Public/Hit/ComboSkillHitBox.h"
#include "Components/BoxComponent.h"

//
AHitBoxCollision::AHitBoxCollision(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HitDamage = CreateDefaultSubobject<UBoxComponent>(TEXT("HitDamage"));
	HitDamage->SetupAttachment(RootComponent);// 让本盒子形状附着在根组件之下.
}

// Called when the game starts or when spawned
void AHitBoxCollision::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHitBoxCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 覆写基类的. 拿取盒子形Comp.
UPrimitiveComponent* AHitBoxCollision::GetHitDamage()
{
	return HitDamage;
}

// 影响外观; 设定盒子的extent;
void AHitBoxCollision::SetBoxExtent(const FVector& InNewBoxExtent)
{
	HitDamage->SetBoxExtent(InNewBoxExtent);
}

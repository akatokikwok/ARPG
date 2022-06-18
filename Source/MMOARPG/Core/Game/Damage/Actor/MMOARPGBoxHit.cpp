#include "MMOARPGBoxHit.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

AMMOARPGBoxHit::AMMOARPGBoxHit(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	if (GetHitDamage()) {
		GetHitDamage()->SetHiddenInGame(true);
	}
}
//
void AMMOARPGBoxHit::BeginPlay()
{
	Super::BeginPlay();
}
//
void AMMOARPGBoxHit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 覆写一个回调函数AHitCollision; 当其他武器触碰hitbox时候会引发伤害.
void AMMOARPGBoxHit::HandleDamage(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::HandleDamage(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (GetInstigator() != OtherActor) {// 首先不能打到自己.
		if (OtherActor->IsA(AMMOARPGCharacterBase::StaticClass())) {
			FGameplayEventData EventData;
			EventData.Instigator = GetInstigator();// 施法者
			EventData.Target = OtherActor;// 攻击目标,打到谁了

			/** 接收到伤害时候,会把伤害信息用此API把伤害事件与Tag传出去,传至GA::基类里的AbilityTask_PMAWDamage里 */
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetInstigator(), 
				FGameplayTag::RequestGameplayTag(TEXT("Player.Attack.ComboLinkage")), 
				EventData);
		}
	}
}

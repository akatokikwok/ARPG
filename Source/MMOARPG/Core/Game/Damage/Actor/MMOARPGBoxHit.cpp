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
		if (AMMOARPGCharacterBase* InTarget = Cast<AMMOARPGCharacterBase>(OtherActor)) {// 只承认只接收与人形态的生物hitbox关联.

			if (!InTarget->IsDie()) {/* 防止敌人死亡后还能被用户攻击.*/

				/* 组1个游玩事件参数. */
				FGameplayEventData EventData;
				EventData.Instigator = GetInstigator();// 施法者
				EventData.Target = OtherActor;// 攻击目标,打到谁了

				// 处理任意一个buff, 给所有与刀刃接触到的有效敌人对象（带有GameplayAbilityComponent）添加GATag.
				for (auto& Tmp : AHitCollision::Buffs) {
					/** 接收到伤害时候,会把伤害信息用此API把伤害事件与Tag传出去,传至GA::基类里的AbilityTask_PMAWDamage里 */
					/** 在数据上 专门处理受击, 击伤 */
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						GetInstigator(),
						/*FGameplayTag::RequestGameplayTag(TEXT("Player.Attack.ComboLinkage")),*/
						FGameplayTag::RequestGameplayTag(Tmp),
						EventData
					);
				}
				
				// 查询死亡, 单一执行死亡或者受击挨打.
				if (InTarget->IsDie()) {
					InTarget->PlayDie();
				}
				else {
					// 让挨打的人面朝向 施法者.
					FRotator TargetRot = (-EventData.Instigator->GetActorForwardVector()).ToOrientationRotator();
					InTarget->SetActorRotation(TargetRot);

					// 先去注册人物内部受击ID
					InTarget->SetHitID(AHitCollision::GetHitID());
					// 再执行人物挨打受击.
					InTarget->PlayHit();
				}

				/* 处理受击的逻辑. */
			}
		}
	}
}

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
		if (AMMOARPGCharacterBase* InPawnSelf = Cast<AMMOARPGCharacterBase>(GetInstigator())) {/* 施法者得存在.*/

			if (AMMOARPGCharacterBase* InTarget = Cast<AMMOARPGCharacterBase>(OtherActor)) {// 只承认只接收与人形态的生物hitbox关联.
				if (InTarget->GetCharacterType() != InPawnSelf->GetCharacterType() || 1) {/* 防止同类人打同类人. 0代表卡PVP开关. */

					if (!InTarget->IsDie()) {/* 防止敌人死亡后还能被用户攻击.*/
						
						/* 仅在服务器上执行.*/
						if (InTarget->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer) == true) {
							
							// 不论挨打者是否死亡都需要传递受击ID, 注册目标的受击序号.
							InTarget->SetHitID(GetHitID());

							/** 组1个游玩事件参数. */
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
// 							static const FName Player_State_Hit_Box_Name(TEXT("Character.State.Hit"));
// 							UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), FGameplayTag::RequestGameplayTag(Player_State_Hit_Box_Name), EventData);
						}

// 						// 查询死亡, 单一执行死亡或者受击挨打.
// 						if (InTarget->IsDie()) {
// 							InTarget->PlayDie();
// 						}
// 						else {
// 							if (GetHitID() != INDEX_NONE) {/* 受击号写为-1,则挨打的人不会有承击动画.*/
// 								// 让挨打的人面朝向 施法者.
// 								FRotator TargetRot = (-EventData.Instigator->GetActorForwardVector()).ToOrientationRotator();
// 								InTarget->SetActorRotation(TargetRot);
// 								
// 								// 先去注册人物内部受击ID
// 								InTarget->SetHitID(AHitCollision::GetHitID());
// 								
// 								// 再执行人物挨打受击.
// 								InTarget->PlayHit();
// 							}
// 						}
					}
				}
			}
		}
	}
}

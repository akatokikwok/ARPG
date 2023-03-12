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
	if (GetInstigator() != OtherActor) {// 首先不能打到自己.
		if (AMMOARPGCharacterBase* InPawnSelf = Cast<AMMOARPGCharacterBase>(GetInstigator())) {/* 施法者得存在.*/

			if (AMMOARPGCharacterBase* InTarget = Cast<AMMOARPGCharacterBase>(OtherActor)) {// 只承认只接收与人形态的生物hitbox关联.
				if (InTarget->GetCharacterType() != InPawnSelf->GetCharacterType() || 1) {/* 防止同类人打同类人. 0代表卡PVP开关. */

					/* 仅在服务器上执行.*/
					if (InTarget->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer) == true) {

						// 已死亡的对象不可以再被攻击
						if (InTarget->IsDie()) {
							return;
						}

						// 已被攻击过的对象不可以被二次攻击产生伤害,仅允许一次.
						if (AHitCollision::IsExist(InTarget)) {
							return;
						}

						// 检查欲攻击的对象 已激活霸体效果
						if (InTarget->IsActiveGameplayEffectTags(TEXT("Player.Buff.SuperArmor"))) {
							// 播放霸体效果
							GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("玩家处于闪避霸体状态,敌人或者野怪攻击不会对其造成伤害!"));
							return;
						}

						// 不论挨打者是否死亡都需要传递受击ID, 注册目标的受击序号.
						InTarget->SetHitID(GetHitID());

						/** 组1个游玩事件参数. */
						FGameplayEventData EventData;
						EventData.Instigator = GetInstigator();// 施法者
						EventData.Target = OtherActor;// 攻击目标,打到谁了

						// 如果人恰好在释放闪避技能, 则拿取闪避的技能状态标签
						FName InDodgeTag = InTarget->DodgeTags();
						/// 对象被攻击打到的时候如果这个对象处于闪避技能状态下,则会播放闪避残影
						if (!InDodgeTag.IsNone() && InTarget->IsExitActiveTag(InDodgeTag)) {
							InTarget->PlayResidualShadowMulticast();
						}
						/// 发现对方发动了格挡
						else if (InTarget->IsExitActiveTag(TEXT("Player.Skill.Block"))) {
							// 注册专属的格挡受击ID:10
							InTarget->SetHitID(10);

							// 攻击对方,对方开启格挡振刀信号, 则自己会被振刀反馈
							if (InTarget->IsVibratingKnife()) {
								InPawnSelf->SetHitID(2);// 临时设定section为2的后退踉跄受击
								InPawnSelf->PlayHit();// 播放受击
							}

							// 播放被攻击目标的子弹时间效果(如树怪攻击人造成伤害盒子,人发动格挡, 人放慢动作,持续零点几秒.
							// "Player.Skill.Block"被传递给回调 UMMOARPGGameplayAbility::OnDamageGameplayEvent(FGameplayTag InGameplayTag, FGameplayEventData Payload)
							// 然后把此格挡tag关联的蓝图内的Block1Buff生效,继而诱发GEEC,放慢时间生效
							UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InTarget, FGameplayTag::RequestGameplayTag(TEXT("Player.Skill.Block")), EventData);

							// 对方格挡了, 也需要播对方格挡受击的动画效果
							InTarget->PlayHit();
						}
						/// 对方没躲闪,就向其注册1个击中标签和注册击中EventData.
						else {
							if (!AHitCollision::Buffs.IsEmpty()) {
								// 处理任意一个buff, 给所有与刀刃接触到的有效敌人对象（带有GameplayAbilityComponent）添加GATag.
								for (auto& AnyBuffName : AHitCollision::Buffs) {
									/** 接收到伤害时候,会把伤害信息用此API把伤害事件与Tag传出去,传至GA::基类里的AbilityTask_PMAWDamage里 */
									/** 在数据上 专门处理受击, 击伤 */
									UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
										GetInstigator(),
										FGameplayTag::RequestGameplayTag(AnyBuffName),/* 默认会在蒙太奇蓝图里配置1个连击标签 FGameplayTag::RequestGameplayTag(TEXT("Player.Attack.ComboLinkage")),*/
										EventData);
								}
							}
							else if (!AHitCollision::BuffTags.IsEmpty()) //BuffTags
							{
								for (auto& AnyBuffGASTag : BuffTags) {
									UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), AnyBuffGASTag, EventData);
								}
							}
							//static const FName Player_State_Hit_Box_Name(TEXT("Character.State.Hit"));
							//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), FGameplayTag::RequestGameplayTag(Player_State_Hit_Box_Name), EventData);
						}
					}
				}

				Super::HandleDamage(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
			}
		}
	}
}

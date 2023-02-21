#include "AnimNotify/AnimNotify_Attack.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "Hit/ComboSkillHitBox.h"
#include "Hit/ComboSkillHitCapsule.h"
#include "Hit/ComboSkillHitCustom.h"
#include "Hit/ComboSkillHitSphere.h"
#include "GameFramework/Character.h"

UAnimNotify_Attack::UAnimNotify_Attack()
	: Super()
{
	HitObjectClass = AHitBoxCollision::StaticClass();// 默认设置为盒子碰撞.

 	bHitBox = true;
	bSpawnCollisionOnServer = true;// 默认仅让hitbox在服务器生成.

 	bHitCapsule = false;
 	bHitCustom = false;
 	bHitSphere = false;
 	BoxExtent = FVector(32.f);
 	SphereRadius = 32.f;
 	CapsuleHalfHeight = 44.f;
 	CapsuleRadius = 22.f;

	LifeTime = 4.f;
	InSocketName = TEXT("OpenFire");
}

FString UAnimNotify_Attack::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

// 重写Notify.
void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (AActor* InSimpleCombatCharacter = Cast<AActor>(MeshComp->GetOuter())) {
		FVector ComponentLocation = MeshComp->GetSocketLocation(InSocketName);
		FRotator ComponentRotation = MeshComp->GetSocketRotation(InSocketName);

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Instigator = Cast<APawn>(InSimpleCombatCharacter);// 施法者设定为自己.
		
		if (InSimpleCombatCharacter->GetWorld() != nullptr) {

			/* 满足任一条件才会 生成Hitbox, 由于bSpawnCollisionOnServer默认为TRUE,所以这一段仅在服务器上生成. */
			if (!bSpawnCollisionOnServer || InSimpleCombatCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {
				/** 生成一个碰撞物hitbox, 大概位于刀尖上的socket上 */
				if (AHitCollision* HitCollision = InSimpleCombatCharacter->GetWorld()->SpawnActor<AHitCollision>(HitObjectClass, ComponentLocation, ComponentRotation, ActorSpawnParameters)) {
					
					// 若允许将Box绑定在角色身上开火点(实际上是手动添加的一个socket)上..
					if (bBind == true) {
						HitCollision->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
					}

					// 优先注册受击ID(在蓝图蒙太奇动画里手动赋值).
					HitCollision->SetHitID(HitID);// 给socket上的这个碰撞物写入一个受击ID.
					// 为hitbox写入一组buff名字.
					HitCollision->SetBuffs(Buffs);
					// 为hitbox写入一组buff名字.
					HitCollision->SetBuffs(BuffTags);

					// 注册受击ID完了之后再启用碰撞.
					HitCollision->Collision(true);

					// 按四种形状细分, 真正的碰撞数据尺寸都在这一步.
					if (HitCollision->GetHitDamage()) {/* 若形状comp确实存在也就是可以转换成确切形状的hitbox. */

						FVector RelativeLocation = HitCollision->GetHitDamage()->GetRelativeLocation();
						HitCollision->SetLifeSpan(LifeTime);// 设定hitbox寿命
						HitCollision->SetHitDamageRelativePosition(RelativeLocation + RelativeOffsetLocation);// 设定hitbox的3D相对位置.

						// 设定各外形hitbox的形状.
						if (AHitBoxCollision* InBox = Cast<AHitBoxCollision>(HitCollision)) {
							InBox->SetBoxExtent(BoxExtent);
						}
						else if (AHitCapsuleCollision* InCapsule = Cast<AHitCapsuleCollision>(HitCollision)) {
							InCapsule->SetCapsuleHalfHeight(CapsuleHalfHeight);
							InCapsule->SetCapsuleRadius(CapsuleRadius);
						}
						else if (AHitSphereCollision* InSphere = Cast<AHitSphereCollision>(HitCollision)) {
							InSphere->SetRadius(SphereRadius);
						}
						else if (AHitCustomCollision* InCustom = Cast<AHitCustomCollision>(HitCollision)) {
							// to do.
						}
					}
				}
			}
		}
		//
	}
}

#if WITH_EDITORONLY_DATA
// 覆写此notify在编辑器蒙太奇中的显示颜色.
FLinearColor UAnimNotify_Attack::GetEditorColor()
{
	return FLinearColor::Red;
}
#endif

#if WITH_EDITOR
// 覆写自UObject的虚方法; 它负责在编辑器里修改字段后重新影响刷新生效.
void UAnimNotify_Attack::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	/* 首先核验欲更改的字段名是不是"HitObjectClass" */
	/* IsA() 意为 某个东西是不是某种类型.*/
	if (PropertyChangedEvent.GetPropertyName().ToString() == TEXT("HitObjectClass")) {
		if (HitObjectClass.GetDefaultObject()->IsA(AHitBoxCollision::StaticClass())) {/* 若是盒形碰撞种类,则仅开放更改盒形字段, 其余的类推. */
			bHitBox = true;
			bHitCapsule = false;
			bHitCustom = false;
			bHitSphere = false;
		}
		else if (HitObjectClass.GetDefaultObject()->IsA(AHitCapsuleCollision::StaticClass())) {
			bHitCapsule = true;
			bHitBox = false;
			bHitCustom = false;
			bHitSphere = false;
		}
		else if (HitObjectClass.GetDefaultObject()->IsA(AHitCustomCollision::StaticClass())) {
			bHitCustom = true;
			bHitBox = false;
			bHitCapsule = false;
			bHitSphere = false;
		}
		else if (HitObjectClass.GetDefaultObject()->IsA(AHitSphereCollision::StaticClass())) {
			bHitSphere = true;
			bHitBox = false;
			bHitCapsule = false;
			bHitCustom = false;
		}
	}
}
#endif

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
void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (AActor* InSimpleCombatCharacter = Cast<AActor>(MeshComp->GetOuter())) {
		FVector ComponentLocation = MeshComp->GetSocketLocation(InSocketName);
		FRotator ComponentRotation = MeshComp->GetSocketRotation(InSocketName);

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Instigator = Cast<APawn>(InSimpleCombatCharacter);// 施法者设定为自己.
		// 生成1个碰撞物.
		if (AHitCollision* HitCollision = InSimpleCombatCharacter->GetWorld()->SpawnActor<AHitCollision>(HitObjectClass, ComponentLocation, ComponentRotation, ActorSpawnParameters)) {
			HitCollision->SetLifeSpan(LifeTime);
		}
		
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

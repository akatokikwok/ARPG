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
// 	bHitBox = true;
// 	bHitCapsule = false;
// 	bHitCustom = false;
// 	bHitSphere = false;
// 	BoxExtent = FVector(32.f);
// 	SphereRadius = 32.f;
// 	CapsuleHalfHeight = 44.f;
// 	CapsuleRadius = 22.f;
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
	if (ACharacter* InSimpleCombatCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
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
FLinearColor UAnimNotify_Attack::GetEditorColor()
{
	return FLinearColor::Red;
}
#endif

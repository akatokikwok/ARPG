#include "Core/Element/SimpleFootIK.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

FSimpleFootIK::FSimpleFootIK()
	: Character(NULL)
	, TraceDistance(50.f)
	, InterpSpeed(18.f)
	, TraceStart_bias(50.0f)
	, bPendingKill(false)
{

}

void FSimpleFootIK::Tick(float DeltaTime)
{
	for (auto& Tmp :  IKInfos) {
		// 本只脚的理想偏移.
		float OffsetTarget = this->FootTrace(Tmp.Key, TraceDistance);

		// 本只脚当前偏移 往 理想偏移 执行渐变插值.
		Tmp.Value.Offset = FMath::FInterpTo(Tmp.Value.Offset, OffsetTarget, DeltaTime, InterpSpeed);
	}
}

void FSimpleFootIK::Init(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceDistance /*= 50.f*/, float InInterpSpeed /*= 18.f*/, float InTraceStart /*= 50.f*/)
{
	Character = InCharacter;
	TraceDistance = InTraceDistance;
	InterpSpeed = InInterpSpeed;
 	TraceStart_bias = InTraceStart;
	for (auto& BoneName_single : InBoneNames) {
		IKInfos.Add(BoneName_single, FFootIKInfo());
	}
}

float FSimpleFootIK::FindOffset(const FName& InKeyName)
{
	if (FFootIKInfo* Info = FindFootIKInfo(InKeyName)) {
		return Info->Offset;
	}

	return 0.f;
}

FFootIKInfo* FSimpleFootIK::FindFootIKInfo(const FName& InKeyName)
{
	return IKInfos.Find(InKeyName);
}

float FSimpleFootIK::FootTrace(const FName& BoneName, float InTraceDistance)
{
	if (Character != nullptr) {
		if (IsValid(Character)) {// 人被销毁了.
 			bPendingKill = true;
			Character = nullptr;
			return 0.0f;
		}

		float CharacterLocZ = Character->GetActorLocation().Z;
		float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		// 射线起点.
		FVector StartLoc = Character->GetMesh()->GetSocketLocation(BoneName) + FVector(0, 0, TraceStart_bias);
		// 射线终点.
		FVector EndLoc = FVector(StartLoc.X, StartLoc.Y,
			CharacterLocZ - (HalfHeight + InTraceDistance)
		);

		// 执行LineTrace.
		FHitResult HitResult;
		TArray<AActor*> Ignores;
		if (UKismetSystemLibrary::LineTraceSingle(
			Character->GetWorld(),
			StartLoc,
			EndLoc,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			Ignores,
			mFootTraceDrawDebugType,
			HitResult,
			true))
		{
			/**
			 * HitResult.Location 是从上往下打的射线打到地面或者硬质地板的交合点.
			 * HitResult.TraceEnd 是忽略地板阻挡本来预计要打到的点(即人为预设的检测终点.)
			 * (HitResult.Location - HitResult.TraceEnd).Size() - InTraceDistance 求得真正的 脚位移到地板上的那段offset.
			 */
			return (HitResult.Location - HitResult.TraceEnd).Size() - InTraceDistance;// 脚相对于地板的偏移,这是个负值.
		}
	}

	return 0.0f;// 检测失败返回0
}

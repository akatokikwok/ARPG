#include "SimpleCombatBPLibrary.h"
#include "SimpleCombat.h"
#include "Manage/ComboCountManage.h"

USimpleCombatBPLibrary::USimpleCombatBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void USimpleCombatBPLibrary::ComboPlay(UObject* WorldContextObject, TSubclassOf<UUI_ComboCount> InClass)
{
	// 使用计数单例, 构建出连打计数UI并同步执行UI表现
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
		FComboCountManage::Get()->Play(World, InClass);
	}
}

// 销毁连打计数单例; 目的是为了防止第二次打开插件崩溃
void USimpleCombatBPLibrary::ComboTextDestroy()
{
	FComboCountManage::Destroy();
}

/** 生成闪避残影Actor并设定存续时长.  */
AResidualShadowActor* USimpleCombatBPLibrary::SpawnResidualShadow(UObject* WorldContextObject, TSubclassOf<AResidualShadowActor>ResidualShadowActorClass, USkeletalMeshComponent* InComponentToCopy, 
	float InZOffset, const FVector& Location, const FRotator& InRot, 
	float InLiftTime /*= 1.f*/)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;// 碰撞设定为总是生成.

		FVector NewLocation = FVector(Location.X, Location.Y, Location.Z + InZOffset);
		FRotator NewRot = FRotator(InRot.Pitch, InRot.Yaw - 90.f, InRot.Roll);

		if (AResidualShadowActor* InRS = World->SpawnActor<AResidualShadowActor>(
			ResidualShadowActorClass, NewLocation, NewRot, SpawnParameters))
		{
			// 设定闪避残影持续时间
			InRS->SetLifeSpan(InLiftTime);
			// 刷新闪避残影(需要指定1个骨骼)
			InRS->UpdateResidualShadow(InComponentToCopy);

			return InRS;
		}
	}
	return NULL;
}

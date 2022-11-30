#include "MMOARPGAISpawnPoint.h"
#include "../Character/Core/MMOARPGCharacterBase.h"
#include "../MMOARPGGameState.h"

AMMOARPGAISpawnPoint::AMMOARPGAISpawnPoint()
	: CharacterID(INDEX_NONE)
	, Lv(1)
	, RebirthTime(4.f)
	, CurrentRebirthTime(RebirthTime)
	, bScopeSpawnAI(false)
	, SpawnAIRadius(500.f)
	, SpawnAINumberInRange(3)
{
	PrimaryActorTick.bCanEverTick = true;

	bNetLoadOnClient = false;// 强迫命令刷怪器只在服务器存在, 客户端不存在
}

void AMMOARPGAISpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {/* 必须是服务器上的逻辑.*/
		SpawnAICharacter(CharacterID, Lv);
	}
}

void AMMOARPGAISpawnPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void AMMOARPGAISpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* 监听池子为空就开始扣除重生时间并主动生成怪物. */
	if (ListeningObjectPool.IsEmpty()) {
		CurrentRebirthTime -= DeltaTime;
		if (CurrentRebirthTime <= 0.f) {
			SpawnAICharacter(CharacterID, Lv);
			CurrentRebirthTime = RebirthTime;
		}
	}
	/* 把已死亡的从监听池子里剔除掉. */
	else {
		TArray<AMMOARPGCharacterBase*> RemoveObject;
		for (auto& Tmp : ListeningObjectPool) {
			if (Tmp->IsDie()) {
				RemoveObject.Add(Tmp);
			}
		}
		for (auto& Tmp : RemoveObject) {
			ListeningObjectPool.Remove(Tmp);
		}
	}
}
////////////////////////////////////////////////////////////////////////// protected:

// 生成多少级的哪个ID的怪物.
void AMMOARPGAISpawnPoint::SpawnAICharacter(int32 InCharacterID, int32 InLV)
{
	if (GetWorld()) {
		if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
			// 让GS读DTRow里的 属性集
// 			if (FCharacterAttributeTable* InAttributeTable = InGameState->GetCharacterAttributeTable(InCharacterID)) {
				// 让GS读DTRow里的 角色样式
				if (FCharacterStyleTable* InCharacterTable_Style = InGameState->GetCharacterStyleTable(InCharacterID)) {
					TArray<FVector> Locations;// 所有怪的位置(如果有多个怪).

					if (bScopeSpawnAI) {// 允许刷一批怪?
						for (int32 i = 0; i < SpawnAINumberInRange; i++) {
							FVector Origin = GetActorLocation();
							FVector2D Point = FMath::RandPointInCircle(SpawnAIRadius);
							Locations.Add(Origin + FVector(Point.X, Point.Y, 0.f));
						}
					}
					else {
						Locations.Add(GetActorLocation());
					}

					// 扫描每个怪的预定生成位置, 并构建出怪; 同时注册进存活池子.
					// 生成怪之后强烈不推荐直接同步数据,因为有时间差.需要另一套设计
					for (auto& Location : Locations) {
						if (AMMOARPGCharacterBase* InCharacterBase = GetWorld()->SpawnActor<AMMOARPGCharacterBase>(
							InCharacterTable_Style->MMOARPGCharacterClass,
							Location,
							FRotator::ZeroRotator)) {

							// 让AI角色升等级.
							InCharacterBase->UpdateLevel(InLV);

							ListeningObjectPool.Add(InCharacterBase);// 同时注册进存活池子.
						}
					}
				}
// 			}
		}
	}
}

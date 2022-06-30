#pragma once
#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "MMOARPGAISpawnPoint.generated.h"

class AMMOARPGCharacterBase;

/**
 * AI小怪生成器.
 * 一般存储在服务器上,客户端仅接受同步过来的部分.
 */
UCLASS()
class MMOARPG_API AMMOARPGAISpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
private:
	UPROPERTY()
		TArray<AMMOARPGCharacterBase*> ListeningObjectPool;
public:
	// 怪物ID,需要在蓝图配置.
	UPROPERTY(EditAnywhere, Category = "AISpawn")
		int32 CharacterID;

	UPROPERTY(EditAnywhere, Category = "AISpawn")
		int32 Lv;

	UPROPERTY(EditAnywhere, Category = "AISpawn")
		float RebirthTime;
	
	UPROPERTY(EditAnywhere, Category = "AISpawn")
		float CurrentRebirthTime;

	// 是否半径内合批生成一批怪.
	UPROPERTY(EditAnywhere, Category = "AISpawn")
		bool bScopeSpawnAI;
	
	// 生成半径.
	UPROPERTY(EditAnywhere, Category = "AISpawn", meta = (EditCondition = "bScopeSpawnAI"))
		float SpawnAIRadius;

	// 规定范围内刷出多少只怪.
	UPROPERTY(EditAnywhere, Category = "AISpawn", meta = (EditCondition = "bScopeSpawnAI"))
		int32 SpawnAINumberInRange;

/// //////////////////////////////////////////////////////////////////////////
public:
	AMMOARPGAISpawnPoint();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
protected:
	// 生成多少级的哪个人物号的怪物.(目前蘑菇怪ID号是100)
	void SpawnAICharacter(int32 InCharacterID, int32 InLV);
};
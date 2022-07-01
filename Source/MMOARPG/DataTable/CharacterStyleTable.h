
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStyleTable.generated.h"

class AMMOARPGCharacterBase;

/**
 * 与DataTable搭配使用(本质上是DT的一行).
 * 角色样式.
 */
USTRUCT(BlueprintType)
struct FCharacterStyleTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterStyleTable();

	UPROPERTY(EditDefaultsOnly, Category = "CharacterStyle")
		int32 ID;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterStyle")
		TSubclassOf<AMMOARPGCharacterBase> MMOARPGCharacterClass;// 哪种类型的生命体?
};
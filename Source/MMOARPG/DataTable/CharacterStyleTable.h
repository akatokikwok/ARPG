
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStyleTable.generated.h"

class AMMOARPGCharacterBase;

/**
 * 与DataTable搭配使用(本质上是DT的一行).
 * 某个游戏角色的种类或阵营.
 */
USTRUCT(BlueprintType)
struct FCharacterStyleTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterStyleTable();

	// 游戏人物的ID号
	UPROPERTY(EditDefaultsOnly, Category = "CharacterStyle")
		int32 ID;

	// 游戏人物的姓名.(FTEXT型利于本地化)
	UPROPERTY(EditDefaultsOnly, Category = "CharacterStyle")
		FText CharacterName;
	
	// 游戏人物属于哪种C++种类的生命体?
	UPROPERTY(EditDefaultsOnly, Category = "CharacterStyle")
		TSubclassOf<AMMOARPGCharacterBase> MMOARPGCharacterClass;//  游戏人物属于哪种C++种类的生命体?
};
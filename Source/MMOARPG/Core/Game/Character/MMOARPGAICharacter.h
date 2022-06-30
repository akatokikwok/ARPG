#pragma once
#include "CoreMinimal.h"
#include "Core/MMOARPGCharacterBase.h"
#include "MMOARPGAICharacter.generated.h"

/**
 * AI人物, 继承自人物基类.
 * 设计此AI类的初衷是为了认DT为数据源,而非同步服务器的数据源
 */
UCLASS()
class MMOARPG_API AMMOARPGAICharacter : public AMMOARPGCharacterBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

};
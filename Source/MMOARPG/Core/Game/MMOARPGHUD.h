#pragma once
#include "CoreMinimal.h"
#include "../Common/Core/GamePlay/HUDBase.h"
#include "SimpleDrawTextType.h"
#include "../../MMOARPGGameType.h"
#include "MMOARPGHUD.generated.h"
class UUI_GameMain;
class UUI_ComboCount;
class ADrawText;

// 高亮技能槽
DECLARE_MULTICAST_DELEGATE_OneParam(FBorderHeightDisplayDelegate, EMMOARPGSkillType);

/**
 * 游戏主HUD.
 */
UCLASS()
class MMOARPG_API AMMOARPGHUD : public AHUDBase
{
	GENERATED_BODY()
public:
	// 事件: 高亮技能槽, 需要指明该技能槽的技能分型
	FBorderHeightDisplayDelegate BorderHeightDisplayDelegate;
	// 事件: 复位技能槽已有的高亮
	FSimpleMulticastDelegate ResetHeightDisplayDelegate;

private:
	UPROPERTY()
		TSubclassOf<UUI_GameMain> GameMainClass;

	UPROPERTY()
		TSubclassOf<UUI_ComboCount> ComboCountClass;// 连打计数蓝图模板

	UPROPERTY()
		TSubclassOf<ADrawText> DrawTextClass;

public:
	AMMOARPGHUD();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	// 连打计数UI执行效果
	void PlayComboCount();

public:
	// 生成漂浮伤害字体
	ADrawText* SpawnDrawText(const FVector InLocation, const FString& InText, const FLinearColor& Color, float Percentage, EInfoAnimType Type = EInfoAnimType::TYPE_DEFAULT, int32 PrefixStyleID = -1, bool bBackground = false);

private:
	UUI_GameMain* GameMain;
};

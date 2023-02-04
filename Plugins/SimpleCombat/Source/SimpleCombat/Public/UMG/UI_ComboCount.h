#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ComboCount.generated.h"
class UTextBlock;
class UProgressBar;

/**
 * Combo连打计数 UI控件
 */
UCLASS()
class SIMPLECOMBAT_API UUI_ComboCount : public UUserWidget
{
	GENERATED_BODY()
public:
	UUI_ComboCount(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	// 设定连击计数text
	void SetComboText(const int32 Num);
	// 播放名为"PlayText"的UMG动画
	void PlayAnim();
	// 控件消亡
	void Die();
	// 设定UI最大存续时长
	void SetMaxDeltaTime(float InNewMaxDeltaTime);

protected:
	UWidgetAnimation* GetNameWidgetAnimation(const FString& InWidgetAnimName);
	void PlayWidgetAnim(const FString& InWidgetAnimName);

protected:
	float TimeDelta;
	float MaxDeltaTime;
	FTimerHandle DieDelayTimeHandle;

protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ComboText;// 连击计数

	UPROPERTY(meta = (BindWidget))
		UProgressBar* ComboAttackBar;// 
};
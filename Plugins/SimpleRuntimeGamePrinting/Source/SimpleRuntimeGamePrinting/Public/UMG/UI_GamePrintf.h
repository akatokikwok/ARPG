#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleRuntimeGamePrintingType.h"
#include "UI_GamePrintf.generated.h"
class UWidgetAnimation;
class URichTextBlock;

/**
 * GamePrint富文本UI
 */
UCLASS()
class SIMPLERUNTIMEGAMEPRINTING_API UUI_GamePrintf : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct();
	virtual void NativeDestruct();

public:
	// 接口: 查找并拿取控件的指定动画,可能返空
	UWidgetAnimation* GetNameWidgetAnimation(const FString& InWidgetAnimName);

	// 执行播放控件动画, 需指定UMG动画具体名称
	void PlayWidgetAnim(const FString& InWidgetAnimName);

	//
	UFUNCTION()
		void PlayTextAnim();

	//
	void SetText(const FText& InText);

protected:
	// 富文本textblock
	UPROPERTY(meta = (BindWidget))
		URichTextBlock* TextBlock;
};
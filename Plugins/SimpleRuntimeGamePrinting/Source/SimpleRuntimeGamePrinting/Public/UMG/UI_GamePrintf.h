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

	// 播放一段UMG动画, 名字是"PlayText"
	UFUNCTION()
		void PlayTextAnim();

	// 设置字体
	void SetText(const FText& InText);

	// 设置字体, 需要富文本表内设置的具体颜色
	void SetText(const FText& InText, const FString& InColor);

	// 设置字体, 需要一个FPrintSlot的绘制空间
	void SetText(const SimpleRuntimeGamePrinting::FPrintSlot& InMySlot);

	// 以路径ID映射来设置图片
	UFUNCTION()
		void SetImage(const FString& InImgID);

protected:
	// 富文本textblock
	UPROPERTY(meta = (BindWidget))
		URichTextBlock* TextBlock;
};
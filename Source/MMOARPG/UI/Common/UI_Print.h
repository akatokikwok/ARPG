// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "Components/TextBlock.h"
#include "UI_Print.generated.h"

/**
 * 专用于打印的控件.
 */
UCLASS()
class MMOARPG_API UUI_Print : public UUI_Base
{
	GENERATED_BODY()
	
public:
	// 播放字体动画(如淡入淡出,显隐).
	void PlayTextAnim();

	// 给文字设置具体内容.
	void SetText(const FText& InText);

private:
	// 携带内容的文字.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MsgLog;
};

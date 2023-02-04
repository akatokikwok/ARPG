#pragma once

#include "CoreMinimal.h"
#include "../../../../Core/UI_Base.h"
#include "UI_ICODragDrog.generated.h"

class UImage;

/**
 * 可拖拽的技能图标
 */
UCLASS()
class MMOARPG_API UUI_ICODragDrog : public UUI_Base
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		UImage* ICON;

public:
	// 给图标设置一张图
	void DrawICON(UTexture2D* InICON);
};
#pragma once

#include "CoreMinimal.h"
#include "../../../../Core/UI_Base.h"
#include "UI_DragDrog.generated.h"

/**
 * 拖拽行为的中间层
 * 覆盖全屏幕,表现在拖拽中的Logo如果在这个控件上松开
 */
UCLASS()
class MMOARPG_API UUI_DragDrog : public UUI_Base
{
	GENERATED_BODY()
public:
	/** 拖拽行为松开 */
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
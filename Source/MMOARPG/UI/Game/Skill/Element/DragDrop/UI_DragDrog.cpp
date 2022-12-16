#include "UI_DragDrog.h"
#include "UI_DragDrog.h"
#include "../UI_SkillSlot.h"
#include "Blueprint/DragDropOperation.h"

/** 拖拽行为松开 */
bool UUI_DragDrog::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation) {// 若之前执行的拖拽行为有Payload
		if (UUI_SkillSlot* MyInventorySlot = Cast<UUI_SkillSlot>(InOperation->Payload)) {// 提取拖拽过程中的技能槽
			MyInventorySlot->VisibleIcon();
		}
	}

	return true;
}
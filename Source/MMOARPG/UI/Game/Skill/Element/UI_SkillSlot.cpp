#include "UI_SkillSlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "DragDrop/UI_ICODragDrog.h"
#include "Styling/SlateBrush.h"
#include "../../../../Core/Game/Character/MMOARPGCharacter.h"

int32 UUI_SkillSlot::PlayerSkillNumber = 0;

UUI_SkillSlot::UUI_SkillSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bMappingKey(false)
	, KeyNumber(INDEX_NONE)
{

}

void UUI_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (bMappingKey) {
		KeyNumber = ++PlayerSkillNumber;
		FString PlayerSkillName = FString::Printf(TEXT("PlayerSkill_%i"), PlayerSkillNumber);
		GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(*PlayerSkillName, IE_Pressed, this, &UUI_SkillSlot::OnClickedWidget);

		// 暂定5个键位, 超出了则重置映射键位
		if (PlayerSkillNumber >= 5) {
			PlayerSkillNumber = 0;
		}
	}
}

void UUI_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_SkillSlot::OnClickedWidget()
{
	UUI_Base::Print(FString::FromInt(KeyNumber));

	if (KeyNumber > 0) {
		if (AMMOARPGCharacter* InCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacter>()) {
			if (InCharacter->GetActionState() == ECharacterActionState::FIGHT_STATE) {// 仅当进入战斗行为状态
				// 服务端执行技能形式的技能攻击(需指定一个槽号)
				InCharacter->SKillAttackOnServer(KeyNumber);
			}
		}
	}
}

/** 侦测鼠标键按下 */
FReply UUI_SkillSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent()) {// 判断是鼠标右键执行了触摸
		FReply Reply = FReply::Handled();
		
		TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
		if (SlateWidgetDrag.IsValid()) {// 检查触碰的UMG是有意义的
			// 此刻标记并侦测为拖拽行为
			Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
			return Reply;
		}
	}

	return FReply::Handled();
}

/** 识别到是拖拽行为 */
void UUI_SkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (this->ICODragDrogClass != nullptr) {// 拖拽ICON显示元素类不为空
		if (UUI_ICODragDrog* ICODragDrogUMGIns = CreateWidget<UUI_ICODragDrog>(GetWorld(), ICODragDrogClass)) {// 构建1个技能图标Ins
			/* New一个拖拽操作行为*/
			if (UDragDropOperation* InDropOperation = NewObject<UDragDropOperation>(GetTransientPackage(), UDragDropOperation::StaticClass())) {
				InDropOperation->SetFlags(RF_StrongRefOnFrame);// 防止GC
				ICODragDrogUMGIns->DrawICON(Cast<UTexture2D>(SlotIcon->Brush.GetResourceObject()));// 设置图图片

				InDropOperation->DefaultDragVisual = ICODragDrogUMGIns;// 填充拖拽行为
				InDropOperation->Payload = this;// 填充拖拽行为
				OutOperation = InDropOperation;// 注册出参

				//this->HiddenIcon();// 隐藏技能图片
			}
		}
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

/** 拖拽行为松开 */
bool UUI_SkillSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	// 拖拽结果
	bool bDrop = false;

	if (InOperation) {// 接收到有入参拖拽操作
		if (AMMOARPGCharacter* InCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacter>()) {
			/* 接收到上一步拖拽行为传入的Operation, 拿到入参拖拽的UMG控件*/
			if (UUI_SkillSlot* MyInventorySlot = Cast<UUI_SkillSlot>(InOperation->Payload)) {
				return true;
			}
		}
	}

	return bDrop;
}

void UUI_SkillSlot::HiddenIcon()
{
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
}

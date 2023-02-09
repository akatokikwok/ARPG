#include "UI_Tip.h"
#include "Components/RichTextBlock.h"

void UUI_Tip::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_Tip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UUI_Tip::SetText(const FText &InText)
{
	TextBlock->SetText(InText);
}

bool UUI_Tip::IsText()const
{
	return !TextBlock->GetText().ToString().IsEmpty();
}
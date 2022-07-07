#include "UI_CharacterHealthWidget.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "UUI_CharacterHealthWidget"

// 设定血条持有者的姓名
void UUI_CharacterHealthWidget::SetCharacterName(const FText& InCharacterName)
{
	if (CharacterName) {
		CharacterName->SetText(InCharacterName);
	}
}

void UUI_CharacterHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_CharacterHealthWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_CharacterHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

#undef LOCTEXT_NAMESPACE
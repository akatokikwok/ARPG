#include "UMG/UI_GamePrintf.h"
#include "Components/RichTextBlock.h"

void UUI_GamePrintf::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_GamePrintf::NativeDestruct()
{
	Super::NativeDestruct();
}

/** 接口: 查找并拿取控件的指定动画,可能返空. */
UWidgetAnimation* UUI_GamePrintf::GetNameWidgetAnimation(const FString& InWidgetAnimName)
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass())) {
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;

		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate(
			[&](const UWidgetAnimation* OurAnimation) {
				return OurAnimation->GetFName().ToString() == (InWidgetAnimName + FString("_INST"));
			});

		return *MyTempAnimation;
	}

	return nullptr;
}

// 播放指定UMG动画
void UUI_GamePrintf::PlayWidgetAnim(const FString& InWidgetAnimName)
{
	if (UWidgetAnimation* MyTempAnimation = this->GetNameWidgetAnimation(InWidgetAnimName)) {
		UUserWidget::PlayAnimation(MyTempAnimation);
	}
}

void UUI_GamePrintf::PlayTextAnim()
{
	PlayWidgetAnim(TEXT("PlayText"));
}

void UUI_GamePrintf::SetText(const FText& InText)
{
	TextBlock->SetText(InText);
}

// 设置字体
void UUI_GamePrintf::SetText(const FText& InText, const FString& InColor)
{
	TextBlock->SetText(FText::FromString(
		SimpleRuntimeGamePrinting::FPrintSlot()
		.AddSlot()
		.Color(InColor)
		[
			InText.ToString()
		].ToString()
	));
}

// 设置字体
void UUI_GamePrintf::SetText(const SimpleRuntimeGamePrinting::FPrintSlot& InMySlot)
{
	TextBlock->SetText(FText::FromString(
		InMySlot.ToString()
	));
}

// 以路径ID映射来设置图片
void UUI_GamePrintf::SetImage(const FString& InImgID)
{
	TextBlock->SetText(FText::FromString(
		SimpleRuntimeGamePrinting::FPrintSlot()
		.AddSlot()
		.Image(InImgID)
		.ToString()
	));
}
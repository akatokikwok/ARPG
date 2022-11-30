#include "Widget/Slate/SlateElement/SAttributeLogBase.h"

void SAttributeLogBase::Construct(const FArguments& InArgs, const FSimplePreDebugPrintf& InMyLog)
{
	ChildSlot
	[
		/** 拼出玩家1对玩家2执行了什么行为, 数值是多少 */
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString(InMyLog.CharacterNameActive))
			.ColorAndOpacity(FSlateColor(FLinearColor::Blue))
		]
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString(InMyLog.EventString))
			.ColorAndOpacity(FSlateColor(FLinearColor::Yellow))
		]
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString(InMyLog.CharacterNamePassive))
			.ColorAndOpacity(FSlateColor(FLinearColor::Green))
		]
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT(" The calculated value is ")))
		]
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString(InMyLog.Value))
			.ColorAndOpacity(FSlateColor(FLinearColor::Red))
		]
	];
}

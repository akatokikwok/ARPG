#include "Widget/Slate/SlateElement/SAttributeLogBase.h"

void SAttributeLogBase::Construct(const FArguments& InArgs, const FSimplePreDebugPrintf& InMyLog)
{
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SNew(STextBlock)
			
		]
// 		+ SHorizontalBox::Slot().AutoWidth()
// 		[
// 			SNew(STextBlock)
// 			.Text(FText::FromString(InMyLog.EventString))
// 			.ColorAndOpacity(FSlateColor(FLinearColor::Yellow))
// 		]
// 		+ SHorizontalBox::Slot().AutoWidth()
// 		[
// 			SNew(STextBlock)
// 			.Text(FText::FromString(InMyLog.CharacterNamePassive))
// 			.ColorAndOpacity(FSlateColor(FLinearColor::Green))
// 		]
// 		+ SHorizontalBox::Slot().AutoWidth()
// 		[
// 			SNew(STextBlock)
// 			.Text(FText::FromString(TEXT(" The calculated value is ")))
// 		]
// 		+ SHorizontalBox::Slot().AutoWidth()
// 		[
// 			SNew(STextBlock)
// 			.Text(FText::FromString(InMyLog.Value))
// 			.ColorAndOpacity(FSlateColor(FLinearColor::Red))
// 		]
	];
}

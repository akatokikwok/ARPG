#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"

#define LOCTEXT_NAMESPACE "SSimepleNumbericalDeductionLog"

void SSimepleNumbericalDeductionLog::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(BoxList, SVerticalBox)
	];
}

void SSimepleNumbericalDeductionLog::AddLog(const FString& InContent)
{
	this->AddLog(FText::FromString(InContent));
}

void SSimepleNumbericalDeductionLog::AddLog(const FText& InContent)
{
	if (BoxList.IsValid()) {
		BoxList->AddSlot()
		[
			SNew()
		];
	}
}

void SSimepleNumbericalDeductionLog::ClearLog()
{
	if (BoxList.IsValid()) {
		BoxList->ClearChildren();
	}
}

void SSimepleNumbericalDeductionLog::SaveAsText()
{

}

#undef LOCTEXT_NAMESPACE
#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"
#include "Widget/Slate/SlateElement/SAttributeLogBase.h"

#define LOCTEXT_NAMESPACE "SSimepleNumbericalDeductionLog"

void SSimepleNumbericalDeductionLog::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(BoxList, SVerticalBox)
	];

	// 仅测试代码
	AddLog("Hello0");
	AddLog("Hello1");
	AddLog("Hello2");
	AddLog("Hello3");
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
			SNew(SAttributeLogBase, InContent)
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
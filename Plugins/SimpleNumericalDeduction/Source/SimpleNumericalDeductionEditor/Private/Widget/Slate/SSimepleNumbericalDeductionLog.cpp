#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"
#include "Widget/Slate/SlateElement/SAttributeLogBase.h"

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
		BoxList->AddSlot().Padding(4.f, 2.f, 4.f, 2.f).HAlign(EHorizontalAlignment::HAlign_Left).AutoHeight()
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

void SSimepleNumbericalDeductionLog::Generate()
{
	// 仅测试代码
	for (size_t i=0; i < 100; ++i) {
		AddLog("Hello0");
		AddLog("Hello1");
		AddLog("Hello2");
		AddLog("Hello3");
	}
}

#undef LOCTEXT_NAMESPACE
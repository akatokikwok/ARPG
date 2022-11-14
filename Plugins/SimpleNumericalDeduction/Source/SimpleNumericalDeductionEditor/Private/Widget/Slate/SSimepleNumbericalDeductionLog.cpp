#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"
#include "Widget/Slate/SlateElement/SAttributeLogBase.h"
#include "Log/SimpleNumericalDeductionLog.h"

#define LOCTEXT_NAMESPACE "SSimepleNumbericalDeductionLog"

void SSimepleNumbericalDeductionLog::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(BoxList, SVerticalBox)
	];

}

bool SSimepleNumbericalDeductionLog::IsTickableInEditor() const
{
	return true;
}

void SSimepleNumbericalDeductionLog::Tick(float DeltaTime)
{
	if (LogContainer.IsPop()) {
		FSimplePreDebugPrintf MyLog;
		LogContainer.Pop(MyLog);

		this->AddLog(MyLog);
	}
}

TStatId SSimepleNumbericalDeductionLog::GetStatId() const
{
	return TStatId();
}

// 塞入1个预打印日志行
void SSimepleNumbericalDeductionLog::AddLog(const FSimplePreDebugPrintf& MyLog)
{
	if (BoxList.IsValid()) {
		BoxList->AddSlot().Padding(4.f, 2.f, 4.f, 2.f).HAlign(EHorizontalAlignment::HAlign_Left).AutoHeight()
		[
			SNew(SAttributeLogBase, MyLog)
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
// 	// 仅测试代码
// 	for (size_t i=0; i < 100; ++i) {
// 		AddLog("Hello0");
// 		AddLog("Hello1");
// 		AddLog("Hello2");
// 		AddLog("Hello3");
// 	}
}

#undef LOCTEXT_NAMESPACE
#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"
#include "Widget/Slate/SlateElement/SAttributeLogBase.h"
#include "Log/SimpleNumericalDeductionLog.h"
#include "Settings/SNDObjectSettings.h"
#include "Settings/SNDNumericalBalanceDebugSettings.h"

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
			SNew(SAttributeLogBase, MyLog)// 槽位里写入1个日志行
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
// 	for (size_t i = 0; i < 100; ++i) {
// 		FSimplePreDebugPrintf PrintfLog;
// 		PrintfLog.CharacterNameActive = TEXT("小明");
// 		PrintfLog.CharacterNamePassive = TEXT("小王");
// 		PrintfLog.EventString = TEXT("造成伤害");
// 		PrintfLog.Value = TEXT("2000");
// 
// 		AddLog(PrintfLog);
// 	}
	
	if (const USNDObjectSettings* SNDObjectSettings = GetDefault<USNDObjectSettings>()) {

		// 寻找感兴趣的 某张Table下的所有属性数据
		auto FindData = [&](const FString& InKey) ->const TArray<FDeduceAttributeData>* {
			for (auto& Tmp : SNDObjectSettings->AttributeDatas) {
				if (Tmp.TableName.ToString() == InKey) {
					return &Tmp.AttributeDatas;
				}
			}
			return nullptr;
		};



		if (const USNDNumericalBalanceDebugSettings* SNDNumericalBalanceDebugSettings = GetDefault<USNDNumericalBalanceDebugSettings>()) {
			
			// 扫描任意玩家间的交互活动
			for (auto& Tmp : SNDNumericalBalanceDebugSettings->DebugCharactersInfo) {
				for (auto& TmpActive : Tmp.CharacterActive) {
					// 扫描单个发起者对接受者的行为
					for (auto& TmpPassive : Tmp.CharacterPassive) {
						
						if (true) {

							if (const TArray<FDeduceAttributeData>* Active = FindData(TmpActive.Key.SelectString)) {
								// 拿到主动方的对象数据与承受方的对象数据
								if (const TArray<FDeduceAttributeData>* Passive = FindData(TmpPassive.Key.SelectString)) {
									TMap<FName, float> LvActiveData;

									TMap<FName, float> LvPassiveData;
								}
							}
						}
					}
				}
			}
		}
				
	}
}

#undef LOCTEXT_NAMESPACE
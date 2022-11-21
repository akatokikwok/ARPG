#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"
#include "Widget/Slate/SlateElement/SAttributeLogBase.h"
#include "Log/SimpleNumericalDeductionLog.h"
#include "Settings/SNDObjectSettings.h"
#include "Settings/SNDNumericalBalanceDebugSettings.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"

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
		
		/** 寻找感兴趣的 某张Table下的所有属性数据 */
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
			for (auto& TmpCharsInfo : SNDNumericalBalanceDebugSettings->DebugCharactersInfo) {
				for (auto& TmpActive : TmpCharsInfo.CharacterActive) {
					for (auto& TmpPassive : TmpCharsInfo.CharacterPassive) {
						
						if (true) {
							if (const TArray<FDeduceAttributeData>* Active = FindData(TmpActive.Key.SelectString)) {// 主动玩家表下的所有属性数据
								if (const TArray<FDeduceAttributeData>* Passive = FindData(TmpPassive.Key.SelectString)) {// 被动玩家表下的所有属性数据
									
									/** Lambda: 寻找对应等级下的某条属性数据 */
									auto GetSpecifyLevelData = [](
										int32 InLv, /*等级*/
										const TArray<FDeduceAttributeData>* InDeduceAttributeDatas, /*所有属性集*/
										const FDebugCharacterInfo& InDebugCharacterInfo, /*关联玩家的日志数据*/
										TMap<FName, float>& OutLvData) /*被填充的<属性名, 推导值>容器*/
										->void 
									{
										int32 Lv = InDebugCharacterInfo.Level;
										for (auto& TmpActiveData : *InDeduceAttributeDatas) {
											if (TmpActiveData.DeduceValue.IsValidIndex(Lv)) {
												OutLvData.Add(TmpActiveData.Key, FCString::Atof(*TmpActiveData.DeduceValue[Lv - 1]));
											}
											else {
												return;
											}
										}
									};

									/** 收集主动玩家与被动玩家的数据*/
									TMap<FName, float> LvActiveData;
									GetSpecifyLevelData(TmpActive.Level, Active, TmpActive, LvActiveData);
									TMap<FName, float> LvPassiveData;
									GetSpecifyLevelData(TmpPassive.Level, Passive, TmpPassive, LvPassiveData);

									/** 模拟策略 */
									if (UNumericalAlgorithmExecuteObject* InObject = Cast<UNumericalAlgorithmExecuteObject>(TmpCharsInfo.TestAlgorithmObject->GetDefaultObject())) {

										/* 测试代码, 测试一下伤害值日志打印*/
										float InValue = InObject->GetDamageAlgorithmValue(LvActiveData, LvPassiveData);
										FSimplePreDebugPrintf PrintfLog;
										PrintfLog.CharacterNameActive = TmpActive.Key.SelectString;
										PrintfLog.CharacterNamePassive = TmpPassive.Key.SelectString;
										PrintfLog.EventString = TEXT("--造成伤害--");
										PrintfLog.Value = FString::SanitizeFloat(InValue);
										AddLog(PrintfLog);
									}
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
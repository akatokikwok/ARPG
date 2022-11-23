#include "Widget/Slate/SSimepleNumbericalDeductionLog.h"
#include "Widget/Slate/SlateElement/SAttributeLogBase.h"
#include "Log/SimpleNumericalDeductionLog.h"
#include "Settings/SNDObjectSettings.h"
#include "Settings/SNDNumericalBalanceDebugSettings.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"
#include "SimpleNumericalDeductionType.h"

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
	
	if (const USNDObjectSettings* SNDObjectSettings = GetDefault<USNDObjectSettings>()) {// SNDObjectSettings
		
		/** 1. 寻找感兴趣的 某张Table下的所有属性数据 */
		auto Lambda_FindData = [&](const FString& InKey) ->const TArray<FDeduceAttributeData>* {
			for (auto& Tmp : SNDObjectSettings->AttributeDatas) {
				if (Tmp.TableName.ToString() == InKey) {
					return &Tmp.AttributeDatas;
				}
			}
			return nullptr;
		};

		/** 2. Lambda: 寻找某ID某个人物对应等级下的某条属性数据 */
		auto Lambda_GetSpecifyLevelData = [](
			int32 InLv, /*等级*/
			const TArray<FDeduceAttributeData>* InDeduceAttributeDatas, /*所有属性集*/
			const FDebugCharacterInfo& InDebugCharacterInfo, /*关联玩家的日志数据*/
			TMap<FName, float>& OutLvData) /*被填充的<属性名, 推导值>容器*/
			->void {
			for (auto& TmpActiveData : *InDeduceAttributeDatas) {
				if (TmpActiveData.DeduceValue.IsValidIndex(InLv)) {
					OutLvData.Add(TmpActiveData.Key, FCString::Atof(*TmpActiveData.DeduceValue[InLv - 1]));
				}
				else {
					return;
				}
			}
		};

		/** 3. 模拟计算并打印日志行 */
		auto Lambda_SimulationCalculation = [&](
			TSubclassOf<UNumericalAlgorithmExecuteObject> InTestAlgorithmObject,/*指定1个数值推导算法*/
			const FString& InActiveCharacterName,/*主动玩家日志信息名字*/
			const FString& InPassiveCharacterName,/*被动玩家日志信息名字*/
			EActionCharacterEventType EventType,/*玩家对玩家的事件类型*/
			const TMap<FName, float>& InLvActiveData,/*收集到的主动玩家数据*/
			const TMap<FName, float>& InLvPassiveData) /*收集到的被动玩家数据*/
			->void
		{
			// 拿取数值算法对象.
			if (UNumericalAlgorithmExecuteObject* InObject = Cast<UNumericalAlgorithmExecuteObject>(InTestAlgorithmObject->GetDefaultObject())) {
				// 获取玩家对玩家的事件类型
				auto GetEventTypeString = [](EActionCharacterEventType InEventType) ->FString {
					switch (InEventType) {
						case EActionCharacterEventType::DAMAGE_EVENT:
							return TEXT("--造成伤害--");
						case EActionCharacterEventType::TREATMENT_EVENT:
							return TEXT("--提供治疗--");
					}
					return TEXT("-出错-");
				};
				// 获取行为造成的推导数值
				auto GetEventTypeValue = [&](EActionCharacterEventType InEventType) ->float {
					switch (InEventType) {
						case EActionCharacterEventType::DAMAGE_EVENT:
							return InObject->GetDamageAlgorithmValue(InLvActiveData, InLvPassiveData);
						case EActionCharacterEventType::TREATMENT_EVENT:
							return InObject->GetTreatmentAlgorithmValue(InLvActiveData, InLvPassiveData);
					}
					return 0.f;
				};

				/* 测试代码, 测试一下伤害值日志打印*/
				FSimplePreDebugPrintf PrintfLog;
				PrintfLog.CharacterNameActive = InActiveCharacterName;
				PrintfLog.CharacterNamePassive = InPassiveCharacterName;
				PrintfLog.EventString = GetEventTypeString(EventType);
				PrintfLog.Value = FString::Printf(TEXT("[%.2f]"), GetEventTypeValue(EventType));
				AddLog(PrintfLog);
			}
		};

		//////////////////////////////////////////////////////////////////////////
		if (const USNDNumericalBalanceDebugSettings* SNDNumericalBalanceDebugSettings = GetDefault<USNDNumericalBalanceDebugSettings>()) {// SNDNumericalBalanceDebugSettings
			
			/** 扫描任意玩家之间的日志信息交互 */
			for (auto& TmpCharsInfo : SNDNumericalBalanceDebugSettings->DebugCharactersInfo) {// 需要知道有多少个角色信息

				/// 禁用迭代许可策略
				if (!SNDNumericalBalanceDebugSettings->bIterationLevel) {

					for (auto& TmpActive : TmpCharsInfo.CharacterActive) {// 拿到单个主动方日志信息
						for (auto& TmpPassive : TmpCharsInfo.CharacterPassive) {// 拿到单个被动方日志信息

							if (const TArray<FDeduceAttributeData>* Active = Lambda_FindData(TmpActive.Key.SelectString)) {// 主动玩家表下的所有属性数据
								if (const TArray<FDeduceAttributeData>* Passive = Lambda_FindData(TmpPassive.Key.SelectString)) {// 被动玩家表下的所有属性数据

									/** 收集主动玩家与被动玩家的数据*/
									TMap<FName, float> LvActiveData;
									Lambda_GetSpecifyLevelData(TmpActive.Level, Active, TmpActive, LvActiveData);
									TMap<FName, float> LvPassiveData;
									Lambda_GetSpecifyLevelData(TmpPassive.Level, Passive, TmpPassive, LvPassiveData);

									/** 模拟策略 */
									Lambda_SimulationCalculation(TmpCharsInfo.TestAlgorithmObject, TmpActive.Key.SelectString, TmpPassive.Key.SelectString, TmpCharsInfo.EventType, 
										LvActiveData, LvPassiveData);
								}
							}
						}
					}
				}
				/// 开启迭代许可策略
				else {
					/** 依据打印策略执行不同方案 */
					switch (SNDNumericalBalanceDebugSettings->IterativeMethod) {
						// 一对多策略
						case EIterativeDebugPrintMethod::ONE_TO_MANY_METHOD:
						{
							for (auto& TmpActive : TmpCharsInfo.CharacterActive) {// 拿到单个主动方日志信息
								if (const TArray<FDeduceAttributeData>* Active = Lambda_FindData(TmpActive.Key.SelectString)) {// 仅访问一次主动玩家表下的数据
									// 收集主动方玩家属性数据
									TMap<FName, float> LvActiveData;
									Lambda_GetSpecifyLevelData(TmpActive.Level, Active, TmpActive, LvActiveData);

									/** 一个主动方下对应所有的被动方日志信息 */
									for (auto& TmpPassive : TmpCharsInfo.CharacterPassive) {// 拿到单个被动方日志信息
										if (const TArray<FDeduceAttributeData>* Passive = Lambda_FindData(TmpPassive.Key.SelectString)) {// 被动玩家表下的所有属性数据
											// 遍历每个推演等级的情况
											for (int32 i = 1; i <= SNDObjectSettings->DeductionNumber; i++) {
												// 每个推演等级都收集1次被动方玩家属性数据
												TMap<FName, float> LvPassiveData;
												Lambda_GetSpecifyLevelData(i, Passive, TmpPassive, LvPassiveData);

												// 开始模拟计算
												Lambda_SimulationCalculation(TmpCharsInfo.TestAlgorithmObject,	TmpActive.Key.SelectString, TmpPassive.Key.SelectString, 
													TmpCharsInfo.EventType, LvActiveData, LvPassiveData);
											}
										}
									}
								}
							}

							break;
						}
						// 多对多策略
						case EIterativeDebugPrintMethod::MANY_TO_MANY_METHOD:
						{
							for (auto& TmpActive : TmpCharsInfo.CharacterActive) {// 拿到单个主动方日志信息
								if (const TArray<FDeduceAttributeData>* Active = Lambda_FindData(TmpActive.Key.SelectString)) {// 仅访问一次主动玩家表下的数据

									/** 一个主动方下对应所有的被动方日志信息 */
									for (auto& TmpPassive : TmpCharsInfo.CharacterPassive) {// 拿到单个被动方日志信息
										if (const TArray<FDeduceAttributeData>* Passive = Lambda_FindData(TmpPassive.Key.SelectString)) {// 被动玩家表下的所有属性数据
											// 遍历每个推演等级的情况
											for (int32 i = 1; i <= SNDObjectSettings->DeductionNumber; i++) {
												// 每个推演等级都收集1次主动方玩家属性数据
												TMap<FName, float> LvActiveData;
												Lambda_GetSpecifyLevelData(TmpActive.Level, Active, TmpActive, LvActiveData);

												// 每个推演等级都收集1次被动方玩家属性数据
												TMap<FName, float> LvPassiveData;
												Lambda_GetSpecifyLevelData(i, Passive, TmpPassive, LvPassiveData);

												/** 以上就实现了每个等级对每个等级的 主被动双方的比较 */

												// 开始模拟计算
												Lambda_SimulationCalculation(TmpCharsInfo.TestAlgorithmObject, TmpActive.Key.SelectString, TmpPassive.Key.SelectString,
													TmpCharsInfo.EventType, LvActiveData, LvPassiveData);
											}
										}
									}
								}
							}

							break;
						}
						// 多对一策略
						case EIterativeDebugPrintMethod::MANY_TO_ONCE_METHOD:
						{
							for (auto& TmpPassive : TmpCharsInfo.CharacterPassive) {
								if (const TArray<FDeduceAttributeData>* Passive = Lambda_FindData(TmpPassive.Key.SelectString)) {
									TMap<FName, float> LvPassiveData;
									Lambda_GetSpecifyLevelData(TmpPassive.Level, Passive, TmpPassive, LvPassiveData);

									for (auto& TmpActive : TmpCharsInfo.CharacterActive) {
										if (const TArray<FDeduceAttributeData>* Active = Lambda_FindData(TmpPassive.Key.SelectString)) {
											for (int32 i = 1; i <= SNDObjectSettings->DeductionNumber; i++) {
												TMap<FName, float> LvActiveData;
												Lambda_GetSpecifyLevelData(i, Active, TmpActive, LvActiveData);

												// 开始模拟计算
												Lambda_SimulationCalculation(TmpCharsInfo.TestAlgorithmObject, TmpActive.Key.SelectString, TmpPassive.Key.SelectString,
													TmpCharsInfo.EventType, LvActiveData, LvPassiveData);
											}
										}
									}
								}
							}
							
							break;
						}
					}
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
#include "SimpleNumericalDeductionType.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"
#include "Settings/SNDObjectSettings.h"

FDeduceAttributeData::FDeduceAttributeData()
	: Coefficient(0.f)
	, BaseAlgorithm(UNumericalAlgorithmExecuteObject::StaticClass())
{}

FSimpleSelectString::FSimpleSelectString()
{
	if (USNDObjectSettings* SND = const_cast<USNDObjectSettings*>(GetDefault<USNDObjectSettings>())) {
		for (auto& Tmp : SND->AttributeDatas) {
			// 把SNDObj的总数据存入下拉菜单里
			SelectStrings.Add(Tmp.TableName.ToString());// 表名(主角, 仆从, 蘑菇怪)
		}
	}
}

void FDebugPrintfLogContainer::Add(const FString& InCharacterNameActive, const FString& InCharacterNamePassive, const FString& InEventString, const FString& InValue)
{
	FSimplePreDebugPrintf& InLog = Logs.AddDefaulted_GetRef();
	InLog.CharacterNameActive = InCharacterNameActive;
	InLog.CharacterNamePassive = InCharacterNamePassive;
	InLog.EventString = InEventString;
	InLog.Value = InValue;
}

void FDebugPrintfLogContainer::Pop(FSimplePreDebugPrintf& Out)
{
	Out = Logs.Pop();
}

bool FDebugPrintfLogContainer::IsPop()
{
	return Logs.Num() != 0;
}

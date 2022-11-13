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
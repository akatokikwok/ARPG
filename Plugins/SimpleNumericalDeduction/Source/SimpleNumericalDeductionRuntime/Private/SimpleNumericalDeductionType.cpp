#include "SimpleNumericalDeductionType.h"
#include "UObject/NumericalAlgorithmExecuteObject.h"

FDeduceAttributeData::FDeduceAttributeData()
	: Coefficient(0.f)
	, BaseAlgorithm(UNumericalAlgorithmExecuteObject::StaticClass())
{}
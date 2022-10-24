#include "UObject/NumericalAlgorithmExecuteObject.h"

float UNumericalAlgorithmExecuteObject::GetFloatAlgorithmValue(const FNAEParam& InParam)
{
	return K2_GetAlgorithmValue_Float(InParam);
}

int32 UNumericalAlgorithmExecuteObject::GetInt32AlgorithmValue(const FNAEParam& InParam)
{
	return K2_GetAlgorithmValue_Int32(InParam);
}

FString UNumericalAlgorithmExecuteObject::GetStringAlgorithmValue(const FNAEParam& InParam)
{
	return K2_GetAlgorithmValue_String(InParam);
}

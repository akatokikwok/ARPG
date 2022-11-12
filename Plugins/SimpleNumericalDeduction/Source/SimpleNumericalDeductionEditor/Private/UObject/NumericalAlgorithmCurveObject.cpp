#include "UObject/NumericalAlgorithmCurveObject.h"

TArray<FRichCurveEditInfo> UNumbericalAlgorithmCurveObject::GetCurves()
{
	return Curves;
}

void UNumbericalAlgorithmCurveObject::AddCurves(const FRichCurveEditInfo& InCurve)
{
	Curves.Add(InCurve);
}
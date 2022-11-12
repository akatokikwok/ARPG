#pragma once
#include "CoreMinimal.h"
#include "SimpleNumericalDeductionType.h"
#include "Curves/CurveBase.h"
#include "NumericalAlgorithmCurveObject.generated.h"

/**
 * 曲线的uobject; 继承自UCurveBase
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLENUMERICALDEDUCTION_API UNumbericalAlgorithmCurveObject : public UCurveBase
{
	GENERATED_BODY()

public:
	// 获取曲线信息, 可覆写
	virtual TArray<FRichCurveEditInfo> GetCurves() override;
	// 添加曲线到曲线信息集
	void AddCurves(const FRichCurveEditInfo& InCurve);

protected:
	// 富文本, 描述曲线的信息集
	TArray<FRichCurveEditInfo> Curves;
};
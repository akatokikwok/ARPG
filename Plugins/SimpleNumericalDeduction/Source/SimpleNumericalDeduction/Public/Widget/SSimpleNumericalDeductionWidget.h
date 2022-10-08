#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 数值推演控件, 派生自SCompoundWidget.
 */
class SSimpleNumericalDeductionWidget :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSimpleNumericalDeductionWidget)
	{}
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);

public:

private:
	TSharedPtr<class SVerticalBox> VerticalList;
};
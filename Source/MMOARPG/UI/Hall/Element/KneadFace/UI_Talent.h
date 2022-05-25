#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UI_Talent.generated.h"

UCLASS()
class MMOARPG_API UUI_Talent : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();
};
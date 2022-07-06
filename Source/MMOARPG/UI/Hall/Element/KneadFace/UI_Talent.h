#pragma once

#include "CoreMinimal.h"
#include "Core/UI_KneadFaceBase.h"
#include "UI_Talent.generated.h"

UCLASS()
class MMOARPG_API UUI_Talent : public UUI_KneadFaceBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();
};
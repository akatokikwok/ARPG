#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UI_SkinColorButton.generated.h"

class UButton;
class UImage;

UCLASS()
class MMOARPG_API UUI_SkinColorButton : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
		void ColorButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
		UButton* ColorButton;

	UPROPERTY(meta = (BindWidget))
		UImage* ColorImg;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../Core/UI_Base.h"
#include "UI_Tip.generated.h"

class URichTextBlock;

/**
 *
 */
UCLASS()
class MMOARPG_API UUI_Tip : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* TextBlock;

public:
	virtual void NativeConstruct() override;

	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetText(const FText& InText);
	bool IsText()const;
};

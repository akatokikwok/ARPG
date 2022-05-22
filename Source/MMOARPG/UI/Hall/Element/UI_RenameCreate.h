// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../UI/Core/UI_Base.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UI_RenameCreate.generated.h"

/**
 * 负责详细创建舞台人物信息的控件.
 */
UCLASS()
class MMOARPG_API UUI_RenameCreate : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	void SetSlotPosition(const int32 InSlotPosition);

protected:
	UFUNCTION()
		void ClickedCreate_callback();
	UFUNCTION()
		void ClickedCancel_callback();
	UFUNCTION()
		void Callback_ClickedFindName();

protected:
	int32 SlotPosition;

private:
	UPROPERTY(meta = (BindWidget))
		UButton* FindNameButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CreateButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* EditableName;
};

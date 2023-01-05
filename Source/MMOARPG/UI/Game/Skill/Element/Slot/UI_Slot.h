// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_CD.h"
#include "UI_Slot.generated.h"

class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
 //CD字体显示，敲击
UCLASS()
class MMOARPG_API UUI_Slot : public UUI_CD
{
	GENERATED_BODY()

protected:
	//键位显示
	UPROPERTY(meta = (BindWidget))
		UTextBlock* SlotKey;

	UPROPERTY(meta = (BindWidget))
		UTextBlock *SlotCDValue;

	UPROPERTY(meta = (BindWidget))
		UButton *ClickButton;

protected:
	virtual void NativeConstruct() override;
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	//重置Slot
	virtual void ResetSlot();

	//开始进入更新CD的状态
	virtual void StartUpdateCD(float InCD);
	virtual void StartUpdateCD();

	//设置最大CD
	virtual void SetMaxCD(float InCD);
	//结束更新
	virtual void EndUpdateCD();

	//清除CD
	void ClearCD();

	//清除CD
	void FoceClearCD();
public:
	void EnableClickButton(bool bEnable);
	bool IsEnableClickButton();
public:
	//键位设置
	void SetKeyName(const FString& NewKeyName);

	//绘制字体CD
	void DrawSlotCDText(float InSlotCD);
public:

	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget() {}

	UFUNCTION(BlueprintCallable)
	virtual void OnReleasedClickedWidget() {}

	//是否处于CD中
	bool IsCooldown() { return SlotBuild.Cooldown <= 0.f; }
protected:

	struct FSlotBuild
	{
		FSlotBuild()
			:Cooldown(0.f)
			, MaxCooldown(0.f)
		{}

		float Cooldown;
		float MaxCooldown;
	};

	FSlotBuild SlotBuild;
};

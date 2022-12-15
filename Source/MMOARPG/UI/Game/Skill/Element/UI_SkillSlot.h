#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UI_SkillSlot.generated.h"

class UImage;
class UButton;

/**
 * 技能UI插槽
 */
UCLASS()
class MMOARPG_API UUI_SkillSlot : public UUI_Base
{
	GENERATED_BODY()
public:
	// 技能图标Image
	UPROPERTY(meta = (BindWidget))
		UImage* SlotIcon;
	// 技能图标外侧包裹的button
	UPROPERTY(meta = (BindWidget))
		UButton* SkillButton;
	//
	UPROPERTY(EditDefaultsOnly, Category = UI)
		bool bMappingKey;
public:
	UUI_SkillSlot(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// 敲击后的回调
	UFUNCTION()
		void OnClickedWidget();

	// 更新图标
	void Update(const FName& InTagName, UTexture2D* InTexture);

protected:
	/** 这是一些为了实现拖拽功能而覆写的接口 (拖拽技能页里ICON到技能横栏里) */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	// 隐藏图标
	void HiddenIcon();

protected:
	static int32 PlayerSkillNumber;
	int32 KeyNumber;

private:
	// 拖拽ICON显示类
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUI_ICODragDrog> ICODragDrogClass;
};
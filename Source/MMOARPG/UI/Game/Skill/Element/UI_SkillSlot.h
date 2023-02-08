﻿#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "Slot/UI_Slot.h"
#include "../../../../MMOARPGGameType.h"
#include "UI_SkillSlot.generated.h"

class UImage;
class UButton;
class AMMOARPGCharacter;

/**
 * 技能槽数据
 */
struct FWidgetSlotInfo
{
	FWidgetSlotInfo()
		: Tags(NAME_None)
		, Cost(0.f)
		, SkillType(EMMOARPGSkillType::NONE_SKILLS)
	{}

	FName Tags;// 技能名字
	float Cost;// 技能消耗量
	EMMOARPGSkillType SkillType;// 技能分类类型

	// 技能名是否有意义
	bool IsVaild() const { return Tags != NAME_None; };

	// 技能槽内所有属性全部复位
	void Reset()
	{
		Tags = NAME_None;
		Cost = 0.f;
		SkillType = EMMOARPGSkillType::NONE_SKILLS;
	}
};

/**
 * 技能UI插槽
 */
UCLASS()
class MMOARPG_API UUI_SkillSlot : public UUI_Slot
{
	GENERATED_BODY()
public:
	// 拖拽ICON显示类
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUI_ICODragDrog> ICODragDrogClass;

	//
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		bool bMappingKey;

public:
	// 技能分类类型
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		EMMOARPGSkillType SkillType;

	// 可以被保存的键位技能名字
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		FString KeyString;

public:
	UUI_SkillSlot(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// 敲击后的回调
	virtual void OnClickedWidget() override;

	//
	virtual void OnReleasedClickedWidget() override;

	// 刷新skillslot数据
	void Update(const FName& InTagName, UTexture2D* InTexture, float InCost = 0.f, EMMOARPGSkillType InSkillType = EMMOARPGSkillType::GENERAL_SKILLS);

public:/// 一系列接口
	// 获取 技能槽信息
	FWidgetSlotInfo& GetSlotInfo() { return SlotInfo; }
	//
	UTexture2D* GetIcon();
	//
	void SetIcon(UTexture2D* InNewTexture);
	//
	void ResetIcon();
	//
	void HiddenIcon();
	//
	void VisibleIcon();

	// 1个SkillSlot是否来自技能页; 即这种KeyNumber为-1的情况
	bool IsSkillTableSlot()const { return KeyNumber == INDEX_NONE; }

protected:
	/** 这是一些为了实现拖拽功能而覆写的接口 (拖拽技能页里ICON到技能横栏里) */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	// 在服务端方面的"交换行为"-拖拽操作实质逻辑
	void UpdateSwapByServer(UUI_SkillSlot* MyInventorySlot, AMMOARPGCharacter* InCharacter);

	//  在客户端方面的"交换行为"-拖拽操作实质逻辑
	void UpdateSwap(UUI_SkillSlot* MyInventorySlot);

	// 在服务端方面的"移动行为"-拖拽操作实质逻辑
	void UpdateMoveToByServer(UUI_SkillSlot* MyInventorySlot, AMMOARPGCharacter* InCharacter);

	// 在客户端方面的"移动行为"-拖拽操作实质逻辑
	void UpdateMoveTo(UUI_SkillSlot* MyInventorySlot);

public:
	// 是否屏蔽了技能输入
	bool IsShieldSkill();

	// 技能是否满足 足以消耗
	bool IsCost();

protected:
	// 映射的键位号码
	static int32 PlayerSkillNumber;

	int32 KeyNumber;// 技能框内的具体的槽的序号
	FWidgetSlotInfo SlotInfo;// 技能槽数据
};
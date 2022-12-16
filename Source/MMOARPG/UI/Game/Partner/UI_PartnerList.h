// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Skill/Element/DragDrop/UI_DragDrog.h"
#include "UI_PartnerList.generated.h"

class UVerticalBox;
class UUI_PartnerInfo;

/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_PartnerList : public UUI_DragDrog
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CharacterList;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_PartnerInfo> UI_PartnerInfoClass;
public:
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	// 初始化外观.
	void Init();
	// 反复遍历列表里的 人物信息框 , 直至 指定的仿函数操作该任务框失败.
	void CallCharacterListByPredicate(TFunction<bool(UUI_PartnerInfo* Info)> InFun);

};

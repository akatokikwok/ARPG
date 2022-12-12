// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Base.h"

/** 接口: 查找并拿取控件的指定动画,可能返空. */
UWidgetAnimation* UUI_Base::GetNameWidgetAnimation(const FString& InWidgetAnimName)
{
	// 将UClass转换成UWidgetBlueprintGeneratedClass.
	if (UWidgetBlueprintGeneratedClass* WBPGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass())) {
		TArray<UWidgetAnimation*> TArrayAnims = WBPGenerated->Animations;// 借助WBP类实例获取所有控件的动画.

		// 寻找感兴趣的动画.
		UWidgetAnimation** MyTempAnim = TArrayAnims.FindByPredicate(
			[&](const UWidgetAnimation* OurAnim) ->bool {
				// 若"查找到的动画名字" 匹配 "入参指定的动画名字 + _INST后缀", 则视为匹配成功.
				return OurAnim->GetFName().ToString() == (InWidgetAnimName + FString("_INST"));
			}
		);

		// 删除查找到的二级指针,恢复成一级指针.
		return *MyTempAnim;
	}
	// 未匹配合适的控件动画成功则返回空.
	return nullptr;
}

void UUI_Base::PlayWidgetAnim(const FString& InWidgetName)
{
	// 先查后播.
	if (UWidgetAnimation* MyTempAnim = GetNameWidgetAnimation(InWidgetName)) {
		UUserWidget::PlayAnimation(MyTempAnim);
	}
}

void UUI_Base::Print(const FName& InContent)
{
	Print(InContent.ToString());
}

void UUI_Base::Print(const FText& InContent)
{
	Print(InContent.ToString());
}

void UUI_Base::Print(const FString& InContent)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, InContent);
	}
}
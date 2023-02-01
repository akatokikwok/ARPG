#include "UMG/UI_ComboCount.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#define LOCTEXT_NAMESPACE "UUI_ComboCount"

UUI_ComboCount::UUI_ComboCount(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TimeDelta(0.f)
	, MaxDeltaTime(0.f)
{

}

void UUI_ComboCount::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_ComboCount::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_ComboCount::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TimeDelta > 0.f) {
		TimeDelta -= InDeltaTime;

		ComboAttackBar->SetPercent(TimeDelta / MaxDeltaTime);

		if (TimeDelta <= 0.f) {
			TimeDelta = 0.f;

			PlayWidgetAnim("PlayBar");

			/**
			 * 为了修复单例被多个客户端仅一次调用的bug,设定一套解决方案,如下
			 * 且插件内也无法使用多线程技术,所以推荐设立1个删除的handle
			 * 延时销毁上1个,并会在ComboCountManage里创建下一个新的
			 * 不使用字段指针去保留UI控件,而且每次都动态构建1个计数控件
			 * 到一定时间后就销毁掉UI
			 */

			// 存续时长消耗完毕后, 执行延时自我销毁,
			GetWorld()->GetTimerManager().SetTimer(DieDelayTimeHandle, this, &UUI_ComboCount::Die, 2.f);
		}
	}
}

// 设定连击计数text
void UUI_ComboCount::SetComboText(const int32 Num)
{
	//ComboText->SetText(FText::Format(LOCTEXT("ComboText_Key", "连打了 +{0} 次"), Num));
	ComboText->SetText(FText::Format(LOCTEXT("ComboText_Key", "+{0}"), Num));
}

// 播放名为"PlayText"的UMG动画
void UUI_ComboCount::PlayAnim()
{
	this->PlayWidgetAnim("PlayText");
}

// 控件消亡
void UUI_ComboCount::Die()
{
	/**
	 * 为了修复单例被多个客户端仅一次调用的bug,设定一套解决方案,如下
	 * 且插件内也无法使用多线程技术,所以推荐设立1个删除的handle
	 * 不使用字段指针去保留UI控件,而且每次都动态构建1个计数控件
	 * 到一定时间后就销毁掉UI
	 */

	// 清除延时器
	if (GetWorld()->GetTimerManager().TimerExists(DieDelayTimeHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(DieDelayTimeHandle);
	}
	// 移除掉控件自己
	this->RemoveFromParent();
}

// 设定UI最大存续时长
void UUI_ComboCount::SetMaxDeltaTime(float InNewMaxDeltaTime)
{
	MaxDeltaTime = InNewMaxDeltaTime;
	TimeDelta = InNewMaxDeltaTime;
	// 复位成正常透明度
	ComboAttackBar->SetRenderOpacity(1.f);
}

#pragma region Protected
UWidgetAnimation* UUI_ComboCount::GetNameWidgetAnimation(const FString& InWidgetAnimName)
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass())) {
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;

		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate(
			[&](const UWidgetAnimation* OurAnimation) {
				return OurAnimation->GetFName().ToString() == (InWidgetAnimName + FString("_INST"));
			});

		return *MyTempAnimation;
	}
	return nullptr;
}

void UUI_ComboCount::PlayWidgetAnim(const FString& InWidgetAnimName)
{
	if (UWidgetAnimation* FoundAnimation = GetNameWidgetAnimation(InWidgetAnimName)) {
		UUserWidget::PlayAnimation(FoundAnimation);
	}
}
#pragma endregion Protected

#undef LOCTEXT_NAMESPACE
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

			// 执行自动销毁
			GetWorld()->GetTimerManager().SetTimer(DieDelayTimeHandle, this, &UUI_ComboCount::Die, 2.f);
		}
	}
}

// 设定连击计数text
void UUI_ComboCount::SetComboText(const int32 Num)
{
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
	// 清除延时器
	if (GetWorld()->GetTimerManager().TimerExists(DieDelayTimeHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(DieDelayTimeHandle);
	}
	// 移除掉UI
	RemoveFromParent();
}

// 设定UI最大存续时长
void UUI_ComboCount::SetMaxDeltaTime(float InNewMaxDeltaTime)
{
	MaxDeltaTime = InNewMaxDeltaTime;
	TimeDelta = InNewMaxDeltaTime;
	// 重置显示
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
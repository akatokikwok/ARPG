#include "Manage/ComboCountManage.h"
#include "UMG/UI_ComboCount.h"

FComboCountManage* FComboCountManage::Instance = nullptr;

FComboCountManage::FComboCountManage()
	: ComboNumber(0)
	, TimeDelta(2.f)
	, MaxDeltaTime(2.f)
{

}

FComboCountManage* FComboCountManage::Get()
{
	if (!Instance) {
		Instance = new FComboCountManage();
	}
	return Instance;
}

void FComboCountManage::Destroy()
{
	if (Instance) {
		delete Instance;
	}
	Instance = nullptr;
}

void FComboCountManage::Tick(float DeltaTime)
{
	// 消耗连打存续时长
	if (TimeDelta > 0.f) {
		TimeDelta -= DeltaTime;
		if (TimeDelta <= 0.f) {
			TimeDelta = 0.f;// "下次"再按下距离上次间隔过长就把存续时长清零
			ComboNumber = 0;
		}
	}
}

TStatId FComboCountManage::GetStatId() const
{
	return TStatId();
}

// 创建连打计数UI
UUI_ComboCount* FComboCountManage::GetComboCount(UWorld* InWorld, TSubclassOf<UUI_ComboCount> InClass)
{
	if (InWorld && InClass) {
		// 构建1个连打计数UI控件,并上屏
		if (UUI_ComboCount* MyComboCountPtr = CreateWidget<UUI_ComboCount>(InWorld, InClass)) {
			MyComboCountPtr->AddToViewport(100);
			return MyComboCountPtr;
		}
	}
	return NULL;
}

// 构建出连打计数UI并同步执行UI表现
void FComboCountManage::Play(UWorld* InWorld, TSubclassOf<UUI_ComboCount> InClass)
{
	if (UUI_ComboCount* InComboCount = GetComboCount(InWorld, InClass)) {
		TimeDelta = 2.f;
		
		ComboNumber++;// 每执行一次函数,即自增计数

		// 相应地, 对UI进行同步表现
		InComboCount->SetComboText(ComboNumber);
		InComboCount->SetMaxDeltaTime(TimeDelta);
		InComboCount->PlayAnim();
	}
}
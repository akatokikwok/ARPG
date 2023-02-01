#include "SimpleCombatBPLibrary.h"
#include "SimpleCombat.h"
#include "Manage/ComboCountManage.h"

USimpleCombatBPLibrary::USimpleCombatBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void USimpleCombatBPLibrary::ComboPlay(UObject* WorldContextObject, TSubclassOf<UUI_ComboCount> InClass)
{
	// 使用计数单例, 构建出连打计数UI并同步执行UI表现
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
		FComboCountManage::Get()->Play(World, InClass);
	}
}

// 销毁连打计数单例; 目的是为了防止第二次打开插件崩溃
void USimpleCombatBPLibrary::ComboTextDestroy()
{
	FComboCountManage::Destroy();
}

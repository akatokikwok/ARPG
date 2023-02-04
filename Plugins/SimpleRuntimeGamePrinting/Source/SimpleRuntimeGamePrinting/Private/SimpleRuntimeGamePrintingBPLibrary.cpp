#include "SimpleRuntimeGamePrintingBPLibrary.h"
#include "SimpleRuntimeGamePrinting.h"
#include "UMG/UI_GamePrintf.h"

USimpleRuntimeGamePrintingBPLibrary::USimpleRuntimeGamePrintingBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool USimpleRuntimeGamePrintingBPLibrary::GamePrint(UWorld* InWorld, TSubclassOf<UUI_GamePrintf> GamePrintfClass, const FText& InSlot, float InDieDelayTime /*= 7.0f*/)
{
	if (InWorld && GamePrintfClass) {
		if (UUI_GamePrintf* GamePrintfPtr = CreateWidget<UUI_GamePrintf>(InWorld, GamePrintfClass)) {// 创建出UI控件
			GamePrintfPtr->AddToViewport(100);
			GamePrintfPtr->SetDieDelayTime(InDieDelayTime);// 设置延时时长
			GamePrintfPtr->SetText(InSlot);// 设定字符内容
			GamePrintfPtr->WidgetConstruct();// UI控件开始构造
			return true;
		}
	}
	return false;
}

bool USimpleRuntimeGamePrintingBPLibrary::GamePrint(
	UWorld* InWorld,
	TSubclassOf<UUI_GamePrintf> GamePrintfClass,
	const SimpleRuntimeGamePrinting::FPrintSlot& InSlot,
	float InDieDelayTime)
{
	return GamePrint(InWorld, GamePrintfClass, FText::FromString(InSlot.ToString()), InDieDelayTime);
}

bool USimpleRuntimeGamePrintingBPLibrary::K2_GamePrintText(
	UObject* WorldContextObject,
	TSubclassOf<UUI_GamePrintf> GamePrintfClass,
	const FText& InText,
	float InDieDelayTime)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
		return USimpleRuntimeGamePrintingBPLibrary::GamePrint(World, GamePrintfClass, InText, InDieDelayTime);
	}

	return false;
}

bool USimpleRuntimeGamePrintingBPLibrary::K2_GamePrint(
	UObject* WorldContextObject,
	TSubclassOf<UUI_GamePrintf> GamePrintfClass,
	const TArray<FSimplePrintSlot>& InSlot,
	float InDieDelayTime)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
		// 对一个PrintSlot实例进行构造
		SimpleRuntimeGamePrinting::FPrintSlot PrintSlot;
		for (auto& Tmp : InSlot) {
			// 链式构建实例
			PrintSlot
				.AddSlot()
				.Color(Tmp.Color)
				[
					Tmp.Content
				];

			if (!Tmp.ImgID.IsEmpty()) {
				// 链式构建实例
				PrintSlot
					.AddSlot()
					.Image(Tmp.ImgID);
			}
		}
		return USimpleRuntimeGamePrintingBPLibrary::GamePrint(World, GamePrintfClass, PrintSlot, InDieDelayTime);
	}
	return false;
}
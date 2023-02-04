#include "HUDBase.h"
#include "UMG/UI_GamePrintf.h"
#include "SimpleRuntimeGamePrintingBPLibrary.h"

AHUDBase::AHUDBase()
{
	// 硬加载1张关于 GamePrintf的蓝图资源
	static ConstructorHelpers::FClassFinder<UUI_GamePrintf> GamePrintf_BPClass(TEXT("/Game/UI/SimpleRuntimeGamePrint/UI_GamePrintf_WBP"));
	GamePrintfClass = GamePrintf_BPClass.Class;
}

void AHUDBase::BeginPlay()
{
	Super::BeginPlay();

}

void AHUDBase::Print(const FText& InContent, const FString& InColor)
{
	if (GamePrintfClass) {
		// 创建1个绘制控件文本,并为其链式构造.
		SimpleRuntimeGamePrinting::FPrintSlot PrintSlot;
		PrintSlot.AddSlot()
			.Color(InColor)
			[
				InContent.ToString()
			];

		USimpleRuntimeGamePrintingBPLibrary::GamePrint(
			GetWorld(),
			GamePrintfClass,
			PrintSlot);
	}
}

void AHUDBase::LogPrint(const FText& InContent)
{
	Print(InContent, "white");
}

void AHUDBase::ErrorPrint(const FText& InContent)
{
	Print(InContent, "Red");
}

void AHUDBase::WarningPrint(const FText& InContent)
{
	Print(InContent, "yellow");
}
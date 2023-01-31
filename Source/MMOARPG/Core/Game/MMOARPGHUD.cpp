#include "MMOARPGHUD.h"
#include "../../UI/Game/UI_GameMain.h"
#include "UMG/UI_GamePrintf.h"
#include "SimpleRuntimeGamePrintingBPLibrary.h"
#include "SimpleRuntimeGamePrintingType.h"


AMMOARPGHUD::AMMOARPGHUD()
{
	// 使用FClassFinder找到蓝图资源.
	static ConstructorHelpers::FClassFinder<UUI_GameMain> GameMain_BPClass(TEXT("/Game/UI/Game/UI_GameMain_BP"));
	GameMainClass = GameMain_BPClass.Class;

	// 硬加载1张关于 GamePrintf的蓝图资源
	static ConstructorHelpers::FClassFinder<UUI_GamePrintf> GamePrintf_BPClass(TEXT("/Game/UI/SimpleRuntimeGamePrint/UI_GamePrintf_WBP"));
	GamePrintfClass = GamePrintf_BPClass.Class;
}

void AMMOARPGHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GameMainClass) {
		GameMain = CreateWidget<UUI_GameMain>(GetWorld(), GameMainClass);
		GameMain->AddToViewport();
	}
}

void AMMOARPGHUD::Print(const FText& InContent, const FString& InColor)
{
	if (GamePrintfClass) {
		// 创建一个绘制空间文本,并为其链式构建
		SimpleRuntimeGamePrinting::FPrintSlot PrintSlot;
		PrintSlot
			.AddSlot()
			.Color(InColor)
			[
				InContent.ToString()
			];

		//
		USimpleRuntimeGamePrintingBPLibrary::GamePrint(GetWorld(), GamePrintfClass, PrintSlot);
	}
}

void AMMOARPGHUD::LogPrint(const FText& InContent)
{
	Print(InContent, "white");
}

void AMMOARPGHUD::ErrorPrint(const FText& InContent)
{
	Print(InContent, "Red");
}

void AMMOARPGHUD::WarningPrint(const FText& InContent)
{
	Print(InContent, "yellow");
}

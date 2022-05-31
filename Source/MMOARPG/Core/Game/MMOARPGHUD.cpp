#include "MMOARPGHUD.h"
#include "../../UI/Game/UI_GameMain.h"

AMMOARPGHUD::AMMOARPGHUD()
{
	// 使用FClassFinder找到蓝图资源.
	static ConstructorHelpers::FClassFinder<UUI_GameMain> GameMain_BPClass(TEXT("/Game/UI/Game/UI_GameMain_BP"));
	GameMainClass = GameMain_BPClass.Class;
}

void AMMOARPGHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GameMainClass) {
		GameMain = CreateWidget<UUI_GameMain>(GetWorld(), GameMainClass);
		GameMain->AddToViewport();
	}
}

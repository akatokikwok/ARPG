#include "MMOARPGHUD.h"
#include "../../UI/Game/UI_GameMain.h"
#include "UMG/UI_ComboCount.h"
#include "Manage/ComboCountManage.h"
#include "SimpleDrawTextFunctionLibrary.h"
#include "Actor/DrawText.h"

AMMOARPGHUD::AMMOARPGHUD()
{
	// 使用FClassFinder找到蓝图资源.
	static ConstructorHelpers::FClassFinder<UUI_GameMain> GameMain_BPClass(TEXT("/Game/UI/Game/UI_GameMain_BP"));
	GameMainClass = GameMain_BPClass.Class;

	// FClassFinder硬加载 UI控件资源: 连打计数
	static ConstructorHelpers::FClassFinder<UUI_ComboCount> ComboCount_BPClass(TEXT("/Game/UIComboText/UI_ComboCount_BP"));
	ComboCountClass = ComboCount_BPClass.Class;
}

void AMMOARPGHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GameMainClass) {
		GameMain = CreateWidget<UUI_GameMain>(GetWorld(), GameMainClass);
		GameMain->AddToViewport();
	}
}

void AMMOARPGHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	FComboCountManage::Destroy();// 销毁单例,防止二次打开崩溃
}

void AMMOARPGHUD::PlayComboCount()
{
	// 构建出连打计数UI并同步执行UI表现
	FComboCountManage::Get()->Play(GetWorld(), ComboCountClass);
}

// 该函数是为了解决打包好的项目在攻击角色的时候无法生成字体bug
ADrawText* AMMOARPGHUD::SpawnDrawText(const FVector InLocation, const FString& InText, const FLinearColor& Color, float Percentage, EInfoAnimType Type /*= EInfoAnimType::TYPE_DEFAULT*/, int32 PrefixStyleID /*= -1*/, bool bBackground /*= false*/)
{
	if (DrawTextClass) {
		// 该函数是为了解决打包好的项目在攻击角色的时候无法生成字体bug
		return USimpleDrawTextFunctionLibrary::SpawnDrawTextByClass(
			GetWorld(),
			DrawTextClass,
			InLocation,
			InText, Color, Percentage, Type, PrefixStyleID, bBackground);
	}

	return nullptr;
}
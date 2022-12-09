#include "PlayerControllerBase.h"
#include "SimplePopupUtils.h"
#include "Core/UI_SimplePopup.h"
#include "UObject/ConstructorHelpers.h"

APlayerControllerBase::APlayerControllerBase()
{
	// 加载弹窗UI的蓝图类,并存至字段TSubclass
	static ConstructorHelpers::FClassFinder<UUI_SimplePopup> SimplePopup_BPClass(TEXT("/Game/UI_SimplePopup_BP"));
	SimplePopupClass = SimplePopup_BPClass.Class;

	// 开启光标显示
	bShowMouseCursor = true;
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerControllerBase::CreateWindows(const FText& InButtonName, const FText& InButtonContent, uint8 InProtocol)
{
	if (!IsNetMode(ENetMode::NM_DedicatedServer)) {
		if (SimplePopupClass) {
			SimplePopupUtils::CreatePopup(GetWorld(),
				SimplePopupClass,
				InButtonContent,
				ESimplePopupType::ONE,
				InButtonName,
				FText(),
				100.f,
				0.f,
				FSimpleDelegate::CreateUObject(this, &APlayerControllerBase::OnSureButtonClicked, InProtocol),
				FSimpleDelegate());
		}
	}
}

void APlayerControllerBase::OnSureButtonClicked(uint8 InProtocol)
{

}

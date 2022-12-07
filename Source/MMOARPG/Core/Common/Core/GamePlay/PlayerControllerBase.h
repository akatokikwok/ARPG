#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UUI_SimplePopup;

/**
 * Controller Base.
 */
UCLASS()
class MMOARPG_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerControllerBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** 创建弹窗(所有派生controller均可使用) */
	void CreateWindows(const FText& InButtonName, const FText& InButtonContent, uint8 InProtocol);

protected:
	UFUNCTION()
	virtual void OnSureButtonClicked(uint8 InProtocol);

protected:
	UPROPERTY()
	TSubclassOf<UUI_SimplePopup> SimplePopupClass;
};

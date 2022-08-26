#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "../../SimpleHotUpdate/Source/SimpleHotUpdate/Public/SimpleHotUpdateType.h"
#include "UI_HotUpdateMain.generated.h"
class UTextBlock;
class UProgressBar;
class UVersionControlObject;
/**
 *
 */
UCLASS()
class MMOARPG_API UUI_HotUpdateMain : public UUI_Base
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DownloadedResourceName;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DownloadedDataSize;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* DownloadProgress;

	// 版本控制对象.
	UPROPERTY()
		UVersionControlObject* VersionControlObject;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeDestruct() override;
private:
	// 下载回调(百分比, 资源名字, 资源, 接受的字节)
	UFUNCTION()
	void Downloading(float InValue, const FString& InName, int32 TotalBytes, int32 BytesReceived);

	// 检查版本.
	UFUNCTION()
	void CheckVersion(EServerVersionResponseType InType);
};
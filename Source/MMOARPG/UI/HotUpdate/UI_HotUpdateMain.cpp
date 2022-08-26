#include "UI_HotUpdateMain.h"
#include "Object/SimpleHotUpdateObject.h"
#include "ThreadManage.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "UUI_HotUpdateMain"

void UUI_HotUpdateMain::NativeConstruct()
{
	Super::NativeConstruct();

	// 创建1个版本控制对象.
	VersionControlObject = UVersionControlObject::CreateObject(UVersionControlObject::StaticClass(), GetWorld());
	if (VersionControlObject != nullptr) {
		VersionControlObject->ProgressDelegate.BindUObject(this, &UUI_HotUpdateMain::Downloading);
		VersionControlObject->VersionDelegate.BindUObject(this, &UUI_HotUpdateMain::CheckVersion);
		VersionControlObject->InitVersion();
	}
}

void UUI_HotUpdateMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UUI_HotUpdateMain::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_HotUpdateMain::Downloading(float InValue, const FString& InName, int32 TotalBytes, int32 BytesReceived)
{
	if (DownloadProgress) {
		// 打印下载进度
		DownloadProgress->SetPercent(InValue);
	}
	if (DownloadedResourceName) {
		// 打印资源名字
		DownloadedResourceName->SetText(FText::FromString(InName));
	}
	if (DownloadedDataSize) {
		// 打印资源块大小
		DownloadedDataSize->SetText(
			FText::Format(LOCTEXT("Downloading_DownloadDataSize", "{0}/{1}"), FText::AsMemory(TotalBytes), FText::AsMemory(BytesReceived)
		));
	}
}

void UUI_HotUpdateMain::CheckVersion(EServerVersionResponseType InType)
{
	if (InType == EServerVersionResponseType::EQUAL) {
		// 两秒延迟后跳转至名为 "Login"的地图
		GThread::Get()->GetCoroutines().BindLambda(2.f, [&]() ->void {
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Login"));
			});
	}
}

#undef LOCTEXT_NAMESPACE